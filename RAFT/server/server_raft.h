#ifndef server_raft_h
#define server_raft_h

#include <string>
#include <memory>
#include <sstream>
#include <thread>
#include "../replicas/replicas.h"
#include "../store.h"
#include "../timer.h"
#include "../tcp/tcpacceptor.h"
#include "../tcp/tcpconnector.h"
#include "../tcp/tcpexception.h"

using namespace std;

template<typename TK, typename TV>
class server_raft
{
    int             m_term;
    int             m_status = serverStatus::follower;
    bool            m_started;
    replica         m_self;
    replica         m_leader;
    replicas        m_replicas;
    store<TK, TV>   m_store;

    std::thread     heartBeatSender;
    std::thread     heartBeatWaiter;
    std::thread     handler;
    std::thread     voter;

    timer           m_timer;

    string handleRequest(const std::string&  request);
    std::vector<std::string> sendForAll(const std::string& message);

    bool isMajority(const std::vector<std::string>& results, std::string val);
    bool isMajority(const std::vector<std::string>& results, TV val);
public:
    enum serverStatus
    {
        follower = 0,
        candidate = 1,
        leader = 2,
    };

    server_raft(const replica& self, int status, string replicaspath, string logpath, bool restore = false);
    ~server_raft();

    void start();
    void startHeartBeatSending();
    void startHeartBeatWaiting();
    void startElection();

    void startRequstHandling();
};

template<typename TK, typename TV>
server_raft<TK, TV>::server_raft(const replica& self, int status, string replicaspath, string logpath, bool restore)
    : m_term(1), m_status(status), m_started(false),
      m_self(self), m_leader(self), m_replicas(replicaspath),
      m_store(logpath, restore)
{
    if (restore)
        m_store.showStore();
}

template<typename TK, typename TV>
server_raft<TK, TV>::~server_raft()
{
    if (handler.joinable()) handler.join();
    if (heartBeatSender.joinable()) heartBeatSender.join();
    if (heartBeatWaiter.joinable()) heartBeatWaiter.join();
    if (voter.joinable()) voter.join();
}

template<typename TK, typename TV>
void server_raft<TK, TV>::start()
{
    std::cout << "current term " << m_term << std::endl;
    if (m_started)
        return;

    m_started = true;
    startRequstHandling();
    if (m_status == serverStatus::leader)
        startHeartBeatSending();
    if (m_status == serverStatus::follower)
        startHeartBeatWaiting();
}

template<typename TK, typename TV>
std::string server_raft<TK, TV>::handleRequest(const std::string& request)
{
    clog << request;

    stringstream requestStream(request);
    string method;
    requestStream >> method;

    if (method == "stop")
    {
        m_started = false;
        clog << " -> stop" << endl;
        return "stopped";
    }

    if (method == "vote")
    {
        stringstream responseStream;
        switch(m_status)
        {
            case serverStatus::candidate:
                responseStream << m_self;
                break;
            default:
                int vote_term;
                replica vote_replica;
                requestStream >> vote_term >> vote_replica;
                responseStream << vote_replica;
                break;
        }
        clog << " -> " << responseStream.str() << endl;
        return responseStream.str();
    }

    if (method == "hb")
    {
        int leader_term;
        replica leader_replica;
        requestStream >> leader_term >> leader_replica;


        stringstream responseStream;
        switch(m_status)
        {
            case serverStatus::candidate:
                m_status = serverStatus::follower;
                m_term = leader_term;
                m_leader = leader_replica;

                responseStream << leader_term << " " << m_leader;
                startHeartBeatWaiting();
                break;
            default:
                if (m_term == leader_term)
                {
                    m_leader = leader_replica;
                    responseStream << leader_term << " " << m_leader;
                    m_timer.reset();
                }
                else
                {
                    m_term ++;
                    m_status = serverStatus::candidate;
                    responseStream << m_term << " " << m_self;
                }
                break;
        }
        clog << " -> " << responseStream.str() << endl;
        return responseStream.str();
    }

    if (method == "get")
    {
        TK key;
        requestStream >> key;
        stringstream responseStream;
        switch(m_status)
        {
            case serverStatus::leader:
                responseStream << m_store.get(key);
                break;
            default:
                responseStream << "redirect " << m_leader;
                break;
        }
        clog << " -> " << responseStream.str() << endl;
        return responseStream.str();
    }

    if (method == "del")
    {
        TK key;
        requestStream >> key;
        stringstream responseStream;
        switch(m_status)
        {
            case serverStatus::leader:
                m_store.del(key);
                responseStream << "deleted";
                break;
            default:
                responseStream << "redirect " << m_leader;
                break;
        }
        clog << " -> " << responseStream.str() << endl;
        return responseStream.str();
    }

    if (method == "set")
    {
        TK key;
        TV val;
        requestStream >> key >> val;
        stringstream forwardingRequestStream;
        stringstream responseStream;

        switch(m_status)
        {
            case serverStatus::leader:
                forwardingRequestStream << "syncset " << key << " " << val;
                if (isMajority(sendForAll(forwardingRequestStream.str()), val))
                {
                    m_store.set(key, val);
                    responseStream << m_store.get(key);
                }
                else
                    responseStream << "not applied";
                break;
            default:
                responseStream << "redirect " << m_leader;
                break;
        }
        clog << " -> " << responseStream.str() << endl;
        return responseStream.str();
    }

    if (method == "syncset")
    {
        TV val;
        TK key;
        requestStream >> key >> val;
        stringstream responseStream;
        switch(m_status)
        {
            case serverStatus::follower:
                m_store.set(key, val);
                responseStream << m_store.get(key);
                break;
            default:
                responseStream << "not applied";
                break;
        }
    }

    clog << " -> undef request" << endl;
    return "undef request";
}

template<typename TK, typename TV>
std::vector<string> server_raft<TK, TV>::sendForAll(const std::string& message)
{
    std::vector<std::string> results;
    TCPConnector connector;
    for(auto repl: m_replicas)
    {
        if (repl == m_self)
            continue;
        try
        {
            auto stream = connector.connect(repl);
            if (stream)
            {
                std::clog << message << std::endl;
                stream->send(message);
                ssize_t len;
                char line[256];
                if ((len = stream->receive(line, sizeof(line))) > 0) {
                    line[len] = 0;
                    results.push_back(std::string(line));
                }
                else throw TCPException("can't read result");
            }
            else throw TCPException("can't connect");
        }
        catch(TCPException& tcpe)
        {
            std::clog << "error from " << repl << ": " << tcpe.what() << std::endl;
            results.push_back("");
        }
    }
    return results;
}

template<typename TK, typename TV>
bool server_raft<TK, TV>::isMajority(const std::vector<std::string>& results, TV val)
{
    std::stringstream s;
    s << val;
    int ac = 0;
    for(auto res: results)
        if (res == s.str()) ++ac;
    return ac > (results.size() - 1) / 2;
}

template<typename TK, typename TV>
bool server_raft<TK, TV>::isMajority(const std::vector<std::string>& results, std::string val)
{
    int ac = 0;
    for(auto res: results)
        if (res == val) ++ac;
    return ac > (results.size() - 1) / 2;
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatSending()
{
    heartBeatSender = std::thread([this](){
        while(m_started && m_status == serverStatus::leader)
        { 
            stringstream message;
            message << "hb " << m_term << " " << m_self;
            std::vector<string> resps(sendForAll(message.str()));
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500));
        }
    });
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatWaiting()
{
    m_timer.start();
    heartBeatWaiter = std::thread([this](){
        while(m_started && m_status == serverStatus::follower)
        {
            if(m_timer.isExpired())
            {
                m_status = serverStatus::candidate;
                startElection();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    });
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startElection()
{
    m_timer.clear();
    voter = std::thread([this](){
        while(m_started && m_status == serverStatus::candidate)
        {
            if (m_timer.isExpired())
            {
                stringstream voteMessage;
                voteMessage << "vote " << m_term << m_self;

                if (isMajority(sendForAll(voteMessage.str()), m_self.toString()))
                {
                    m_status = serverStatus::leader;
                    startHeartBeatSending();
                }
                else
                {
                    m_timer.reset();
                }
            }
            else
            {
                std::this_thread::yield();
            }
        }
    });
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startRequstHandling()
{
    handler = std::thread([this](){
        TCPAcceptor acceptor(m_self);
        m_started = acceptor.start();
        while (m_started)
        {
            auto stream = acceptor.accept();
            if (stream) {
                ssize_t len;
                char line[256];
                while ((len = stream->receive(line, sizeof(line))) > 0) {
                    line[len] = 0;
                    string response = handleRequest(string(line));
                    stream->send(response);
                }
            }
        }
    });
}

#endif
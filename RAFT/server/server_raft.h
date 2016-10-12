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
    int             m_status = serverStatus::followeer;
    bool            m_started;
    replica         m_self;
    replica         m_leader;
    replicas        m_replicas;
    store<TK, TV>   m_store;

    std::thread     heartBeatSender;
    std::thread     heartBeatWaiter;
    std::thread     handler;

    timer           m_timer;

    string handleRequest(const std::string&  request);
    std::vector<std::string> sendForAll(const std::string& message);
    bool isMajority(const std::vector<string>& results, TV val);
public:
    enum serverStatus
    {
        followeer = 0,
        candidate = 1,
        leader = 2,
    };

    server_raft(const replica& self, int status, string replicaspath, string logpath, bool restore = false);
    ~server_raft();

    void start();
    void startHeartBeatSending();
    void startHeartBeatWaiting();
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
    if (m_status == serverStatus::followeer)
        startHeartBeatWaiting();
}

template<typename TK, typename TV>
std::string server_raft<TK, TV>::handleRequest(const std::string& request)
{
    clog << request;

    stringstream ss(request);
    string method;
    ss >> method;
    if (method == "stop")
    {
        m_started = false;
        clog << " -> stop" << endl;
        return "stopped";
    }

    if (m_status == serverStatus::candidate)
    {
        clog << " -> rejected" << endl;
        return "rejected";
    }

    if (method == "heartBeat" && m_status == serverStatus::followeer)
    {
        int leader_term;
        replica leader_replica;
        ss >> leader_term >> leader_replica;

        stringstream respstream;
        if (m_term == leader_term)
        {
            m_leader = leader_replica;
            respstream << leader_term << " " << m_leader;
            m_timer.reset();
        }
        else
        {
            m_term ++;
            m_status = serverStatus::candidate;
            respstream << m_term << " " << m_self;
        }
        clog << " -> " << respstream.str() << endl;
        return respstream.str();
    }

    TK key;
    ss >> key;
    if (method == "get")
    {
        stringstream respstream;
        respstream << m_store.get(key);
        clog << " -> " << respstream.str() << endl;
        return respstream.str();
    }
    if (method == "del")
    {
        m_store.del(key);
        clog << " -> deleted" << endl;
        return "deleted";
    }
    if (method == "set")
    {
        TV val;
        ss >> val;

        if((m_status == serverStatus::leader && isMajority(sendForAll(request), val)) ||
           (m_status == serverStatus::followeer))
        {
            m_store.set(key, val);
            stringstream respstream;
            respstream << m_store.get(key);
            clog << " -> " << respstream.str() << endl;
            return respstream.str();
        }
        return "not applied";
    }

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
bool server_raft<TK, TV>::isMajority(const std::vector<string>& results, TV val)
{
    std::stringstream s;
    s << val;
    int ac = 0;
    for(auto res: results)
        if (res == s.str()) ++ac;
    return ac > (results.size() - 1) / 2;
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatSending()
{
    heartBeatSender = std::thread([this](){
        while(m_started && m_status == serverStatus::leader)
        { 
            stringstream message;
            message << "heartBeat " << m_term << " " << m_self;
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
        while(m_started && m_status == serverStatus::followeer)
        {
            if(m_timer.isExpired())
            {
                m_status = serverStatus::candidate;
                cout << "Candidate now!!!!!" << endl;
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
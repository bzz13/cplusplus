#pragma once

#include <string>
#include <memory>
#include <sstream>
#include <thread>
#include <mutex>
#include "../replicas/replicas.h"
#include "../store.h"
#include "../timer.h"
#include "../tcp/tcpacceptor.h"
#include "../tcp/tcpconnector.h"
#include "../tcp/tcpexception.h"
#include "../rpc/server_proto_operation.h"
#include "../rpc/server_proto_parser.h"

using namespace std;

template<typename TK, typename TV>
class server_raft
{
    friend class server_proto_undef<TK, TV>;
    friend class server_proto_stop<TK, TV>;
    friend class server_proto_vote<TK, TV>;
    friend class server_proto_heartbeat<TK, TV>;
    friend class server_proto_get<TK, TV>;
    friend class server_proto_del<TK, TV>;
    friend class server_proto_set<TK, TV>;
    friend class server_proto_syncset<TK, TV>;

    int             m_term;
    int             m_status;
    bool            m_started;
    replica         m_self;
    replica         m_leader;
    replicas        m_replicas;
    store<TK, TV>   m_store;

    std::thread     heartBeatSender;
    std::thread     heartBeatWaiter;
    std::thread     handler;

    std::mutex      m_mtx;

    timer           m_timer;

    std::unique_ptr<replica>    m_vote_for_replica;
    std::unique_ptr<int>        m_vote_for_term;

    string handleRequest(const std::string&  request);
    std::vector<std::string> sendForAll(const std::string& message, const std::string& selfResponse);
    std::vector<std::string> sendForAll(const std::string& message, TV& selfResponseTV);

    bool isMajority(const std::vector<std::string>& results, const std::string& val);
    bool isMajority(const std::vector<std::string>& results, TV& val);
public:
    enum serverStatus
    {
        follower = 0,
        candidate = 1,
        leader = 2,
    };

    server_raft(const replica& self, string replicaspath, string logpath, bool restore = false);
    ~server_raft();

    void start();
    void startHeartBeatSending();
    void startHeartBeatWaiting();
    void startRequstHandling();
};

template<typename TK, typename TV>
server_raft<TK, TV>::server_raft(const replica& self, string replicaspath, string logpath, bool restore)
    : m_term(1), m_status(serverStatus::follower), m_started(false),
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
    if (m_started)
        return;

    clog << "!!!!!NOW FOLLOWER!!!" << endl;

    m_started = true;
    startRequstHandling();
    startHeartBeatWaiting();
    startHeartBeatSending();
}

template<typename TK, typename TV>
std::vector<string> server_raft<TK, TV>::sendForAll(const std::string& message, const std::string& selfResponse)
{
    std::vector<std::string> results;
    TCPConnector connector;
    for(auto repl: m_replicas)
    {
        if (repl == m_self)
        {
            results.push_back(selfResponse);
            continue;
        }
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
std::vector<string> server_raft<TK, TV>::sendForAll(const std::string& message, TV& selfResponseTV)
{
    std::vector<std::string> results;
    TCPConnector connector;
    for(auto repl: m_replicas)
    {
        if (repl == m_self)
        {
            stringstream selfResponse;
            selfResponse << selfResponseTV;
            results.push_back(selfResponse.str());
            continue;
        }
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
bool server_raft<TK, TV>::isMajority(const std::vector<std::string>& results, TV& val)
{
    std::stringstream s;
    s << val;
    std::string ss(s.str());
    int ac = 0;
    for(auto res: results)
        if (res == ss) ++ac;
    return ac >= results.size() - ac;
}

template<typename TK, typename TV>
bool server_raft<TK, TV>::isMajority(const std::vector<std::string>& results, const std::string& val)
{
    int ac = 0;
    for(auto res: results)
        if (res == val) ++ac;
    return ac >= results.size() - ac;
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatSending()
{
    if (!heartBeatSender.joinable())
    {
        heartBeatSender = std::thread([this](){
            while(m_started)
            {
                m_mtx.lock();
                if (m_status == serverStatus::leader)
                {
                    stringstream message;
                    message << "hb " << m_term << " " << m_self;
                    std::vector<string> resps(sendForAll(message.str(), m_term));
                    m_mtx.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 250));
                }
                else
                {
                    m_mtx.unlock();
                    std::this_thread::yield();
                }
            }
        });
    }
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatWaiting()
{
    m_timer.start();
    if (!heartBeatWaiter.joinable())
    {
        heartBeatWaiter = std::thread([this](){
            while(m_started)
            {
                m_mtx.lock();
                if(m_status == serverStatus::follower && m_timer.isExpired())
                {
                    m_term++;
                    m_status = serverStatus::candidate; clog << "!!!!!NOW CANDIDATE!!!" << endl;
                    {
                        stringstream voteMessage;
                        voteMessage << "vote " << m_term << " " << m_self;

                        if (isMajority(sendForAll(voteMessage.str(), m_self.toString()), m_self.toString()))
                        {
                            m_status = serverStatus::leader; clog << "!!!!!NOW LEADER!!!" << endl;
                            startHeartBeatSending();
                        }
                        else
                        {
                            m_status = serverStatus::follower; clog << "!!!!!NOW FOLLOWER!!!" << endl;
                            startHeartBeatWaiting();
                        }
                    }
                    m_mtx.unlock();
                }
                else
                {
                    m_mtx.unlock();
                    std::this_thread::yield();
                }
            }
        });
    }
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

template<typename TK, typename TV>
std::string server_raft<TK, TV>::handleRequest(const std::string& request)
{
    clog << request;

    server_proto_parser<TK, TV> parser;
    auto server_proto_op = parser.parse(request);
    std::string result(server_proto_op->applyTo(this));

    clog << " -> " << result << std::endl;
    return result;
}
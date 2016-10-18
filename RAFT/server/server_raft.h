#pragma once

#include <string>
#include <memory>
#include <sstream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <vector>
#include "../replicas/replicas.h"
#include "../store.h"
#include "../timer.h"
#include "../tcp/tcpacceptor.h"
#include "../tcp/tcpconnector.h"
#include "../tcp/tcpexception.h"
#include "../rpc/server_proto_operation.h"
#include "../rpc/server_proto_parser.h"
#include "server_raft_receiver.h"
#include "server_raft_sender.h"

using namespace std;

template<typename TK, typename TV>
class server_raft
{
    friend class server_proto_undef<TK, TV>;
    friend class server_proto_stop<TK, TV>;
    friend class server_proto_vote_init<TK, TV>;
    friend class server_proto_vote<TK, TV>;
    friend class server_proto_vote_for<TK, TV>;
    friend class server_proto_heartbeat<TK, TV>;
    friend class server_proto_get<TK, TV>;
    friend class server_proto_del<TK, TV>;
    friend class server_proto_set<TK, TV>;
    friend class server_proto_syncset<TK, TV>;

    int                             m_term;
    int                             m_status;
    bool                            m_started;
    replica                         m_self;
    replica                         m_leader;
    replicas                        m_replicas;
    store<TK, TV>                   m_store;
    server_raft_receiver<TK, TV>    m_receiver;
    server_raft_sender              m_sender;

    std::thread     heartBeatSender;
    std::thread     heartBeatWaiter;
    std::thread     handler;

    std::mutex      m_mtx;
    timer           m_timer;

    std::unique_ptr<replica>                    m_vote_for_replica;
    std::unique_ptr<int>                        m_vote_for_term;
    std::unordered_map<int, std::vector<bool>>  m_voting;

    void startHeartBeatSending();
    void startHeartBeatWaiting();
    void startRequstHandling();

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
};

template<typename TK, typename TV>
server_raft<TK, TV>::server_raft(const replica& self, string replicaspath, string logpath, bool restore)
    : m_term(1), m_status(serverStatus::follower), m_started(false),
      m_self(self), m_leader(self), m_replicas(replicaspath),
      m_store(logpath, restore), m_receiver(self), m_sender(self)
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
    m_receiver.startRequestReciving();
    m_sender.startRequestSending();

    startRequstHandling();
    startHeartBeatWaiting();
    startHeartBeatSending();
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
                if (m_status == serverStatus::leader &&
                    m_started)
                {
                    stringstream heartBeatMessage;
                    heartBeatMessage << "hb " << m_self << " " << m_term;
                    m_sender.sendRequest(m_replicas, m_self, heartBeatMessage.str());
                    m_mtx.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 250));
                }
                else
                {
                    m_mtx.unlock();
                }
                std::this_thread::yield();
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
                if(m_status == serverStatus::follower && 
                    m_started && m_timer.isExpired())
                {
                    m_timer.clear();
                    stringstream voteInitMessage;
                    voteInitMessage << "vote_init";
                    m_sender.sendRequest(m_self, voteInitMessage.str());
                }
                m_mtx.unlock();
                std::this_thread::yield();
            }
        });
    }
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startRequstHandling()
{
    if (!handler.joinable())
    {
        handler = std::thread([this](){
            while (m_started)
            {
                m_mtx.lock();
                if (m_receiver.hasRequest() &&
                    m_started)
                {
                    auto operation = m_receiver.getRequest();
                    operation->applyTo(this);
                }
                m_mtx.unlock();
                std::this_thread::yield();
            }
        });
    }
}
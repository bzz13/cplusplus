#pragma once

#include <string>
#include <sstream>
#include <thread>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "../replicas/replicas.h"
#include "../store.h"
#include "../timer.h"
#include "../tcp/tcpstream.h"
#include "../tcp/tcpacceptor.h"
#include "../tcp/tcpconnector.h"
#include "../tcp/tcpexception.h"
#include "../rpc/server_proto_operation.h"
#include "../rpc/server_proto_parser.h"
#include "server_raft_interconnector.h"

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
    friend class server_proto_syncset_for<TK, TV>;

    int                             m_term;
    int                             m_status;
    int                             m_last_applied_index;
    bool                            m_started;
    replica                         m_self;
    replica                         m_leader;
    replicas                        m_replicas;
    store<TK, TV>                   m_store;

    server_raft_interconnector<TK, TV> m_connector;

    timer           m_vote_timer;
    timer           m_hb_timer;

    std::unique_ptr<replica>                    m_vote_for_replica;
    std::unique_ptr<int>                        m_vote_for_term;
    std::unordered_map<int, std::vector<bool>>  m_voting;

    std::unordered_map<int, std::tuple<std::shared_ptr<TCPStream>, TK, TV, std::vector<bool>>> m_syncs;

    std::vector<std::pair<const replica, const std::string>> get_extra_messages_by_current_state();
public:
    enum serverStatus
    {
        follower = 0,
        candidate = 1,
        leader = 2,
    };

    server_raft(const replica& self, std::string replicaspath, std::string logpath, bool restore = false);

    void start();
};

template<typename TK, typename TV>
server_raft<TK, TV>::server_raft(const replica& self, std::string replicaspath, std::string logpath, bool restore)
    : m_term(1), m_status(serverStatus::follower), m_started(false),
      m_self(self), m_leader(self), m_replicas(replicaspath), m_connector(self),
      m_store(logpath, restore),
      m_vote_timer(1000, 5000, 750, 500), m_hb_timer(0, 50, 50, 100)
{
    std::cout << "!!!!!NOW FOLLOWER!!!" << std::endl;
    if (restore)
        m_store.showStore();
    m_last_applied_index = restore ? m_store.lastAppliedIndex() : 0;
}

template<typename TK, typename TV>
void server_raft<TK, TV>::start()
{
    if (m_started)
        return;

    m_started = true;
    m_vote_timer.start();

    while(m_started)
    {
        auto incomingMessage = m_connector.try_get_message();
        if (incomingMessage.first)
            incomingMessage.second->apply_to(this);
        auto extraMessages = get_extra_messages_by_current_state();
        m_connector.send_messages(extraMessages);
    }
}

template<typename TK, typename TV>
std::vector<std::pair<const replica, const std::string>> server_raft<TK, TV>::get_extra_messages_by_current_state()
{
    std::vector<std::pair<const replica, const std::string>> results;
    std::stringstream message;

    switch(m_status)
    {
        case serverStatus::follower:
            if (m_vote_timer.isExpired())
            {
                m_vote_timer.clear();
                results.push_back({m_self, "vote_init"});
            }
            break;
        case serverStatus::leader:
            if (m_hb_timer.isExpired())
            {
                m_hb_timer.reset();
                message << "hb " << m_self << " " << m_term << " " << m_last_applied_index;
                for(auto r: m_replicas)
                    if(r != m_self)
                        results.push_back({r, message.str()});
            }
            break;
        default:
            break;
    }
    return results;
}
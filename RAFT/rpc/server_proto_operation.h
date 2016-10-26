#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include "server_proto_operation.h"
#include "../replicas/replica.h"

template<typename TK, typename TV> class server_raft;

template<typename TK, typename TV>
class server_proto_operation
{
public:
    virtual void apply_to(server_raft<TK, TV>* server) = 0;
};


template<typename TK, typename TV>
class server_proto_undef: protected server_proto_operation<TK, TV>
{
    std::shared_ptr<TCPStream> m_stream;
public:
    server_proto_undef(std::shared_ptr<TCPStream>& stream): m_stream(stream)
    {
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::cout << "undef request" << std::endl;
        m_stream << "undef request";
    }
};

template<typename TK, typename TV>
class server_proto_stop: protected server_proto_operation<TK, TV>
{
    std::shared_ptr<TCPStream> m_stream;
public:
    server_proto_stop(std::shared_ptr<TCPStream>& stream): m_stream(stream)
    {
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        server->m_started = false;
        server->m_connector.stop();
        m_stream << "stopped";
    }
};


template<typename TK, typename TV>
class server_proto_vote_init: protected server_proto_operation<TK, TV>
{
public:
    virtual void apply_to(server_raft<TK, TV>* server)
    {
        server->m_term++;
        server->m_status = server_raft<TK, TV>::serverStatus::candidate;
        std::cout << "!!!!!NOW CANDIDATE!!!" << std::endl;
        server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(server->m_term)));
        server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(server->m_self)));
        server->m_voting.insert({server->m_term, std::vector<bool>()});

        // std::stringstream voteMessage;
        // voteMessage << "vote " << server->m_self << " " << server->m_term;
        // server->m_sender.sendRequest(server->m_replicas, voteMessage.str());
    }
};

template<typename TK, typename TV>
class server_proto_vote: protected server_proto_operation<TK, TV>
{
    replica vote_replica;
    int     vote_term;
public:
    server_proto_vote(std::istream& requestStream)
    { 
        requestStream >> vote_replica >> vote_term;
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        response << "vote_for " << vote_replica << " " << vote_term;
        if (vote_term < server->m_term)
        {
             response << " false";
        }
        else
        {
            if ((!server->m_vote_for_term && !server->m_vote_for_replica) || 
                *(server->m_vote_for_term) < vote_term)
            {
                server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(vote_replica)));
                server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(vote_term)));
                if (*(server->m_vote_for_term) < vote_term && 
                    server->m_status != server_raft<TK, TV>::serverStatus::follower)
                {
                    server->m_status = server_raft<TK, TV>::serverStatus::follower;
                    std::cout << "!!!!!NOW FOLLOWER!!!" << std::endl;
                    server->m_timer.reset();
                }
            }
            if (*(server->m_vote_for_replica) == vote_replica)
                response << " true";
            else
                response << " false";
        }

        std::cout << "-> " << response.str() << std::endl;
        // server->m_sender.sendRequest(vote_replica, response.str());
    }
};


template<typename TK, typename TV>
class server_proto_vote_for: protected server_proto_operation<TK, TV>
{
    replica vote_replica;
    int     vote_term;
    bool    vote_result;

    bool hasMajority(const std::vector<bool>& votes, const int total)
    {
        int count = 0;
        for (auto vote: votes)
            if (vote) ++count;
        return count > total / 2;
    }
public:
    server_proto_vote_for(std::istream& requestStream)
    { 
        std::string vote;
        requestStream >> vote_replica >> vote_term >> vote;
        vote_result = (vote == "true");
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        auto votes = server->m_voting.find(vote_term);
        if (votes != server->m_voting.end())
        {
            if (server->m_term == vote_term)
            {
                votes->second.push_back(vote_result);
                if (hasMajority(votes->second, server->m_replicas.size()))
                {
                    std::cout << "!!!!!NOW LEADER!!!" << std::endl;
                    server->m_status = server_raft<TK, TV>::serverStatus::leader;
                    server->m_voting.erase(vote_term);
                }
                else
                {
                    if (votes->second.size() > server->m_replicas.size() / 2)
                    {
                        server->m_status = server_raft<TK, TV>::serverStatus::follower;
                        std::cout << "!!!!!NOW FOLLOWER!!!" << std::endl;
                        server->m_timer.reset();
                        server->m_voting.erase(vote_term);
                    }
                }
            }
            else
            {
                server->m_voting.erase(vote_term);
            }
        }
    }
};

template<typename TK, typename TV>
class server_proto_heartbeat: protected server_proto_operation<TK, TV>
{
    replica leader_replica;
    int     leader_term;
    int     leader_last_applied_index;
public:
    server_proto_heartbeat(std::istream& requestStream)
    {
        requestStream >> leader_replica >> leader_term >> leader_last_applied_index;
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        response << "hb_for " << leader_replica << " " << leader_term;
        if (leader_term < server->m_term)
        {
            response << " false";
        }
        else
        {
            if (server->m_status != server_raft<TK, TV>::serverStatus::follower)
            {
                server->m_status = server_raft<TK, TV>::serverStatus::follower;
                std::cout << "!!!!!NOW FOLLOWER!!!" << std::endl;
            }
            server->m_term = leader_term;
            server->m_leader = leader_replica;
            server->m_timer.reset();
            response << " true";
        }
        std::cout << "-> " << response.str() << std::endl;
        // server->m_sender.sendRequest(leader_replica, response.str());
    }
};

template<typename TK, typename TV>
class server_proto_heartbeat_for: protected server_proto_operation<TK, TV>
{
    replica hb_replica;
    int     hb_term;
    bool    hb_result;
public:
    server_proto_heartbeat_for(std::istream& requestStream)
    {
        std::string result;
        requestStream >> hb_replica >> hb_term >> result;
        hb_result = (result == "true");
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
    }
};

template<typename TK, typename TV>
class server_proto_get: protected server_proto_operation<TK, TV>
{
    TK key;
    std::shared_ptr<TCPStream> m_stream;
public:
    server_proto_get(std::istream& requestStream, std::shared_ptr<TCPStream>& stream): m_stream(stream)
    {
        requestStream >> key;
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                response << server->m_store.get(key);
                break;
            default:
                response << "redirect " << server->m_leader;
                break;
        }
        std::cout << "-> " << response.str() << std::endl;
        m_stream << response.str();
    }
};

template<typename TK, typename TV>
class server_proto_del: protected server_proto_operation<TK, TV>
{
    TK key;
    std::shared_ptr<TCPStream> m_stream;
public:
    server_proto_del(std::istream& requestStream, std::shared_ptr<TCPStream>& stream): m_stream(stream)
    {
        requestStream >> key;
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                server->m_store.del(key);
                response << "deleted";
                break;
            default:
                response << "redirect " << server->m_leader;
                break;
        }
        std::cout << "-> " << response.str() << std::endl;
        m_stream << response.str();
    }
};

template<typename TK, typename TV>
class server_proto_set: protected server_proto_operation<TK, TV>
{
    TK key;
    TV val;
    std::shared_ptr<TCPStream> m_stream;
public:
    server_proto_set(std::istream& requestStream, std::shared_ptr<TCPStream>& stream): m_stream(stream)
    {
        requestStream >> key >> val;
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        std::stringstream forwardingRequestStream;
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                server->m_syncs[server->m_last_applied_index + 1] = std::make_tuple(m_stream, key, val, std::vector<bool>());
                forwardingRequestStream << "syncset " << (server->m_last_applied_index + 1) << " " << key << " " << val;
                // server->m_sender.sendRequest(server->m_replicas, server->m_self, forwardingRequestStream.str());
                break;
            default:
                response << "redirect " << server->m_leader;
                m_stream << response.str();
                break;
        }
    }
};

template<typename TK, typename TV>
class server_proto_syncset: protected server_proto_operation<TK, TV>
{
    int m_index;
    TK key;
    TV val;
public:
    server_proto_syncset(std::istream& requestStream)
    {
        requestStream >> m_index >> key >> val;
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        response << "syncset_for " << m_index << " ";
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                response << "false";
                break;
            default:
                server->m_last_applied_index = m_index;
                server->m_store.set(key, val);
                response << "true";
                break;
        }
        // server->m_sender.sendRequest(server->m_leader, response.str());
        std::cout << "-> " << response.str() << std::endl;
    }
};

template<typename TK, typename TV>
class server_proto_syncset_for: protected server_proto_operation<TK, TV>
{
    int  m_index;
    bool m_result;

    bool hasMajority(const std::vector<bool>& syncs, const int total)
    {
        int count = 0;
        for (auto sync: syncs)
            if (sync) ++count;
        return (count >= total / 2);
    }
public:
    server_proto_syncset_for(std::istream& requestStream)
    {
        std::string result;
        requestStream >> m_index >> result;
        m_result = (result == "true");
    }

    virtual void apply_to(server_raft<TK, TV>* server)
    {
        std::stringstream response;

        auto findAt = server->m_syncs.find(m_index);
        if (findAt != server->m_syncs.end())
        {
            std::get<3>(server->m_syncs[m_index]).push_back(m_result);

            if (hasMajority(std::get<3>(server->m_syncs[m_index]), server->m_replicas.size() - 1))
            {
                server->m_store.set(std::get<1>(server->m_syncs[m_index]), std::get<2>(server->m_syncs[m_index]));
                server->m_last_applied_index ++;
                std::get<0>(server->m_syncs[m_index]) << "true";
                std::cout << "-> " << "true" << std::endl;
                server->m_syncs.erase(m_index);
            }
            else
            {
                if(std::get<3>(server->m_syncs[m_index]).size() > server->m_replicas.size() / 2)
                {
                    std::get<0>(server->m_syncs[m_index]) << "false";
                    std::cout << "-> " << "false" << std::endl;
                    server->m_syncs.erase(m_index);
                }
            }
        }
    }
};
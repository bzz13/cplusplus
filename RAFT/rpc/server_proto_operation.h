#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include "server_proto_operation.h"
#include "../replicas/replica.h"

template<typename TK, typename TV> class server_raft;
template<typename TK, typename TV> class server_proto_parser;

template<typename TK, typename TV>
class server_proto_operation
{
    friend class server_proto_parser<TK, TV>;
protected:
    std::stringstream   m_responseStream;
    replica             m_message_sender;
public:
    virtual std::string applyTo(server_raft<TK, TV>* server) = 0;
    virtual replica getSender() const { return m_message_sender; }
};


template<typename TK, typename TV>
class server_proto_undef: protected server_proto_operation<TK, TV>
{
public:
    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        std::cerr << "undef request" << std::endl;
        return "";
    }
};

template<typename TK, typename TV>
class server_proto_stop: protected server_proto_operation<TK, TV>
{
public:
    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_started = false;
        server->m_receiver.stopRequestReciving();
        server->m_sender.stopRequestSending();
        std::clog << "stopped" << std::endl;
        return "";
    }
};


template<typename TK, typename TV>
class server_proto_vote_init: protected server_proto_operation<TK, TV>
{
public:
    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_term++;
        server->m_status = server_raft<TK, TV>::serverStatus::candidate;
        std::clog << "!!!!!NOW CANDIDATE!!!" << std::endl;
        server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(server->m_term)));
        server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(server->m_self)));
        server->m_voting.insert({server->m_term, std::vector<bool>()});
        server->m_timer.start();

        std::stringstream voteMessage;
        voteMessage << "vote " << server->m_self << " " << server->m_term;
        server->m_sender.sendRequest(server->m_replicas, voteMessage.str());

        return "";
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

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server_proto_operation<TK, TV>::m_responseStream 
            << "vote_for " << vote_replica << " " << vote_term;
        if (vote_term < server->m_term)
        {
             server_proto_operation<TK, TV>::m_responseStream << " false";
        }
        else
        {
            if ((!server->m_vote_for_term && !server->m_vote_for_replica) || 
                *(server->m_vote_for_term) < vote_term)
            {
                server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(vote_replica)));
                server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(vote_term)));
            }
        }
        if (*(server->m_vote_for_replica) == vote_replica)
            server_proto_operation<TK, TV>::m_responseStream << " true";
        else
            server_proto_operation<TK, TV>::m_responseStream << " false";

        return server_proto_operation<TK, TV>::m_responseStream.str();
    }

    virtual replica getSender() const { return vote_replica; }
};


template<typename TK, typename TV>
class server_proto_vote_for: protected server_proto_operation<TK, TV>
{
    replica vote_replica;
    int     vote_term;
    bool    vote_result;

    bool hasMajority(const std::vector<bool>& votes, const int& total)
    {
        int count;
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

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        std::clog << "<<< fr: " << vote_replica << " msg: " << vote_result << std::endl;
        auto votes = server->m_voting.find(vote_term);
        if (votes != server->m_voting.end())
        {
            votes->second.push_back(vote_result);
            if (hasMajority(votes->second, server->m_replicas.size()))
            {
                std::clog << "!!!!!NOW LEADER!!!" << std::endl;
                server->m_status = server_raft<TK, TV>::serverStatus::leader;
            }
        }
        return "";
    }

    virtual replica getSender() const { return vote_replica; }
};

template<typename TK, typename TV>
class server_proto_heartbeat: protected server_proto_operation<TK, TV>
{
    replica leader_replica;
    int     leader_term;
public:
    server_proto_heartbeat(std::istream& requestStream)
    {
        requestStream >> leader_replica >> leader_term;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server_proto_operation<TK, TV>::m_responseStream << leader_replica << " " << leader_term;
        if (leader_term < server->m_term)
        {
            server_proto_operation<TK, TV>::m_responseStream << " false";
        }
        else
        {
            if (server->m_status != server_raft<TK, TV>::serverStatus::follower)
            {
                server->m_status = server_raft<TK, TV>::serverStatus::follower;
                std::clog << "!!!!!NOW FOLLOWER!!!" << std::endl;
            }
            server->m_term = leader_term;
            server->m_leader = leader_replica;
            server->m_timer.reset();
            server_proto_operation<TK, TV>::m_responseStream << " true";
        }
        return "";
    }

    virtual replica getSender() const { return leader_replica; }
};

template<typename TK, typename TV>
class server_proto_get: protected server_proto_operation<TK, TV>
{
    TK key;
public:
    server_proto_get(std::istream& requestStream)
    {
        requestStream >> key;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_mtx.lock();
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                server_proto_operation<TK, TV>::m_responseStream << server->m_store.get(key);
                break;
            default:
                server_proto_operation<TK, TV>::m_responseStream << "redirect " << server->m_leader;
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::m_responseStream.str();
    }
};

template<typename TK, typename TV>
class server_proto_del: protected server_proto_operation<TK, TV>
{
    TK key;
public:
    server_proto_del(std::istream& requestStream)
    {
        requestStream >> key;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_mtx.lock();
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                server->m_store.del(key);
                server_proto_operation<TK, TV>::m_responseStream << "deleted";
                break;
            default:
                server_proto_operation<TK, TV>::m_responseStream << "redirect " << server->m_leader;
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::m_responseStream.str();
    }
};

template<typename TK, typename TV>
class server_proto_set: protected server_proto_operation<TK, TV>
{
    TK key;
    TV val;
public:
    server_proto_set(std::istream& requestStream)
    {
        requestStream >> key >> val;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        std::stringstream forwardingRequestStream;
        server->m_mtx.lock();
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::leader:
                forwardingRequestStream << "syncset " << key << " " << val;
                if (server->isMajority(server->sendForAll(forwardingRequestStream.str(), val), val))
                {
                    server->m_store.set(key, val);
                    server_proto_operation<TK, TV>::m_responseStream << server->m_store.get(key);
                }
                else
                    server_proto_operation<TK, TV>::m_responseStream << "not applied";
                break;
            default:
                server_proto_operation<TK, TV>::m_responseStream << "redirect " << server->m_leader;
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::m_responseStream.str();
    }
};

template<typename TK, typename TV>
class server_proto_syncset: protected server_proto_operation<TK, TV>
{
    TK key;
    TV val;
public:
    server_proto_syncset(std::istream& requestStream)
    {
        requestStream >> key >> val;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_mtx.lock();
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::follower:
                server->m_store.set(key, val);
                server_proto_operation<TK, TV>::m_responseStream << server->m_store.get(key);
                break;
            default:
                server_proto_operation<TK, TV>::m_responseStream << "not applied";
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::m_responseStream.str();
    }
};
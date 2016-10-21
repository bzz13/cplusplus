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
    virtual void applyTo(server_raft<TK, TV>* server) = 0;
};


template<typename TK, typename TV>
class server_proto_undef: protected server_proto_operation<TK, TV>
{
    std::shared_ptr<TCPStream> m_stream;
public:
    server_proto_undef(std::shared_ptr<TCPStream>& stream): m_stream(stream)
    {
    }

    virtual void applyTo(server_raft<TK, TV>* server)
    {
        std::cerr << "undef request" << std::endl;
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

    virtual void applyTo(server_raft<TK, TV>* server)
    {
        server->m_started = false;
        server->m_receiver.stopRequestReciving();
        server->m_sender.stopRequestSending();
        m_stream << "stopped";
    }
};


template<typename TK, typename TV>
class server_proto_vote_init: protected server_proto_operation<TK, TV>
{
public:
    virtual void applyTo(server_raft<TK, TV>* server)
    {
        server->m_term++;
        server->m_status = server_raft<TK, TV>::serverStatus::candidate;
        std::clog << "!!!!!NOW CANDIDATE!!!" << std::endl;
        server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(server->m_term)));
        server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(server->m_self)));
        server->m_voting.insert({server->m_term, std::vector<bool>()});

        std::stringstream voteMessage;
        voteMessage << "vote " << server->m_self << " " << server->m_term;
        server->m_sender.sendRequest(server->m_replicas, voteMessage.str());
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

    virtual void applyTo(server_raft<TK, TV>* server)
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
                    server->m_status == server_raft<TK, TV>::serverStatus::candidate)
                {
                    server->m_status = server_raft<TK, TV>::serverStatus::follower;
                    std::clog << "!!!!!NOW FOLLOWER!!!" << std::endl;
                    server->m_timer.reset();
                }
            }
            if (*(server->m_vote_for_replica) == vote_replica)
                response << " true";
            else
                response << " false";
        }

        std::clog << "-> " << response.str() << std::endl;
        server->m_sender.sendRequest(vote_replica, response.str());
    }
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

    virtual void applyTo(server_raft<TK, TV>* server)
    {
        std::clog << "<<< fr: " << vote_replica << " msg: " << vote_result << std::endl;
        auto votes = server->m_voting.find(vote_term);
        if (votes != server->m_voting.end())
        {
            if (server->m_term == vote_term)
            {
                votes->second.push_back(vote_result);
                if (hasMajority(votes->second, server->m_replicas.size()))
                {
                    std::clog << "!!!!!NOW LEADER!!!" << std::endl;
                    server->m_status = server_raft<TK, TV>::serverStatus::leader;
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
public:
    server_proto_heartbeat(std::istream& requestStream)
    {
        requestStream >> leader_replica >> leader_term;
    }

    virtual void applyTo(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        response << leader_replica << " " << leader_term;
        if (leader_term < server->m_term)
        {
            response << " false";
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
            response << " true";
        }
        std::clog << "-> " << response.str() << std::endl;
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

    virtual void applyTo(server_raft<TK, TV>* server)
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
        std::clog << "-> " << response.str() << std::endl;
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

    virtual void applyTo(server_raft<TK, TV>* server)
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
        std::clog << "-> " << response.str() << std::endl;
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

    virtual void applyTo(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        std::stringstream forwardingRequestStream;
        // switch(server->m_status)
        // {
            // TODO: implement this
            // case server_raft<TK, TV>::serverStatus::leader:
            //     forwardingRequestStream << "syncset " << key << " " << val;
            //     if (server->isMajority(server->sendForAll(forwardingRequestStream.str(), val), val))
            //     {
                    server->m_store.set(key, val);
                    response << server->m_store.get(key);
            //     }
            //     else
            //         response << "not applied";
            //     break;
            // default:
            //     response << "redirect " << server->m_leader;
            //     break;
        // }
        std::clog << "-> " << response.str() << std::endl;
        m_stream << response.str();
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

    virtual void applyTo(server_raft<TK, TV>* server)
    {
        std::stringstream response;
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::follower:
                server->m_store.set(key, val);
                response << server->m_store.get(key);
                break;
            default:
                response << "not applied";
                break;
        }
        std::clog << "-> " << response.str() << std::endl;
        // server->m_sender.sendRequest(vote_replica, response.str());
    }
};
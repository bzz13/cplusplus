#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "server_proto_operation.h"

template<typename TK, typename TV> class server_raft;
template<typename TK, typename TV> class server_proto_parser;

template<typename TK, typename TV>
class server_proto_operation
{
    friend class server_proto_parser<TK, TV>;
protected:
    std::stringstream responseStream;
public:
    virtual std::string applyTo(server_raft<TK, TV>* server) = 0;
};


template<typename TK, typename TV>
class server_proto_undef: protected server_proto_operation<TK, TV>
{
public:
    virtual std::string applyTo(server_raft<TK, TV>* server)
    {

        return "undef request";
    }
};

template<typename TK, typename TV>
class server_proto_stop: protected server_proto_operation<TK, TV>
{
public:
    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_mtx.lock();
        server->m_started = false;
        server->m_mtx.unlock();
        return "stopped";
    }
};

template<typename TK, typename TV>
class server_proto_vote: protected server_proto_operation<TK, TV>
{
    int     vote_term;
    replica vote_replica;
public:
    server_proto_vote(std::istream& requestStream)
    { 
        requestStream >> vote_term >> vote_replica;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        // server->m_mtx.lock();
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::candidate:
                server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(server->m_self)));
                server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(server->m_term)));
                break;
        }
        if (!server->m_vote_for_replica || !server->m_vote_for_term || *server->m_vote_for_term > vote_term)
        {
            server->m_vote_for_replica = std::move(std::unique_ptr<replica>(new replica(vote_replica)));
            server->m_vote_for_term = std::move(std::unique_ptr<int>(new int(vote_term)));
        }
        server_proto_operation<TK, TV>::responseStream << *(server->m_vote_for_replica);
        // server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::responseStream.str();
    }
};

template<typename TK, typename TV>
class server_proto_heartbeat: protected server_proto_operation<TK, TV>
{
    int     leader_term;
    replica leader_replica;
public:
    server_proto_heartbeat(std::istream& requestStream)
    {
        requestStream >> leader_term >> leader_replica;
    }

    virtual std::string applyTo(server_raft<TK, TV>* server)
    {
        server->m_mtx.lock();
        switch(server->m_status)
        {
            case server_raft<TK, TV>::serverStatus::candidate:
                server->m_status = server_raft<TK, TV>::serverStatus::follower; std::clog << "!!!!!NOW FOLLOWER!!!" << std::endl;
                server->m_term = leader_term;
                server->m_leader = leader_replica;
                server_proto_operation<TK, TV>::responseStream << leader_term << " " << leader_term;
                break;
            default:
                if (server->m_term == leader_term)
                {
                    server->m_leader = leader_replica;
                    server->m_timer.reset();
                    server_proto_operation<TK, TV>::responseStream << leader_term << " " << server->m_leader;
                }
                else
                {
                    server->m_term ++;
                    server->m_status = server_raft<TK, TV>::serverStatus::candidate; std::clog << "!!!!!NOW CANDIDATE!!!" << std::endl;
                    server_proto_operation<TK, TV>::responseStream << server->m_term << " " << server->m_self;
                }
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::responseStream.str();
    }
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
                server_proto_operation<TK, TV>::responseStream << server->m_store.get(key);
                break;
            default:
                server_proto_operation<TK, TV>::responseStream << "redirect " << server->m_leader;
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::responseStream.str();
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
                server_proto_operation<TK, TV>::responseStream << "deleted";
                break;
            default:
                server_proto_operation<TK, TV>::responseStream << "redirect " << server->m_leader;
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::responseStream.str();
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
                    server_proto_operation<TK, TV>::responseStream << server->m_store.get(key);
                }
                else
                    server_proto_operation<TK, TV>::responseStream << "not applied";
                break;
            default:
                server_proto_operation<TK, TV>::responseStream << "redirect " << server->m_leader;
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::responseStream.str();
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
                server_proto_operation<TK, TV>::responseStream << server->m_store.get(key);
                break;
            default:
                server_proto_operation<TK, TV>::responseStream << "not applied";
                break;
        }
        server->m_mtx.unlock();
        return server_proto_operation<TK, TV>::responseStream.str();
    }
};
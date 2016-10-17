#pragma once

#include <queue>
#include <mutex>
#include <memory>
#include <utility> 
#include <thread>
#include "../replicas/replica.h"
#include "../rpc/server_proto_parser.h"
#include "../tcp/tcpacceptor.h"


template<typename TK, typename TV>
class server_raft_receiver
{
    typedef server_proto_operation<TK, TV> spo;
    typedef std::shared_ptr<spo> spt_spo;
    typedef std::shared_ptr<TCPStream> spt_stm;
    typedef std::pair<spt_stm, spt_spo> p;


    bool                        m_started;
    replica                     m_self;
    std::mutex                  m_mtx;
    std::thread                 m_handler;
    TCPAcceptor                 m_acceptor;
    server_proto_parser<TK, TV> m_parser;
    std::queue<p>               m_queue;

public:
    server_raft_receiver(const replica& self);
    ~server_raft_receiver();

    void startRequestReciving();
    void stopRequestReciving();
    bool hasRequest();
    p getRequest();
};

template<typename TK, typename TV>
server_raft_receiver<TK, TV>::server_raft_receiver(const replica& self)
    : m_started(false), m_self(self), m_acceptor(self)
{
}

template<typename TK, typename TV>
server_raft_receiver<TK, TV>::~server_raft_receiver()
{
    if(m_handler.joinable())
        m_handler.join();
}

template<typename TK, typename TV>
void server_raft_receiver<TK, TV>::startRequestReciving()
{
    if (!m_handler.joinable())
    {
        m_handler = std::thread([this](){
            m_started = m_acceptor.start();

            while (m_started)
            {
                auto stream = m_acceptor.accept();
                if (stream) {
                    char request[256];
                    auto len = stream->receive(request, sizeof(request), 50);
                    if (len > 0)
                    {
                        request[len] = 0;
                        auto server_proto_op = m_parser.parse(request);
                        m_mtx.lock();
                        if (m_started)
                        {
                            std::clog << "<<< " << request << std::endl;
                            m_queue.push(std::make_pair(stream, server_proto_op));
                        }
                        m_mtx.unlock();
                    }
                }
            }
        });
    }
}

template<typename TK, typename TV>
void server_raft_receiver<TK, TV>::stopRequestReciving()
{
    m_mtx.lock();
    m_started = false;
    while(!m_queue.empty())
        m_queue.pop();
    m_mtx.unlock();
}


template<typename TK, typename TV>
bool server_raft_receiver<TK, TV>::hasRequest()
{
    bool result;
    m_mtx.lock();
    result = !m_queue.empty();
    m_mtx.unlock();
    return result;
}

template<typename TK, typename TV>
std::pair<std::shared_ptr<TCPStream>, std::shared_ptr<server_proto_operation<TK, TV>>> 
server_raft_receiver<TK, TV>::getRequest()
{
    std::pair<std::shared_ptr<TCPStream>, std::shared_ptr<server_proto_operation<TK, TV>>> result;
    m_mtx.lock();
    result = m_queue.front();
    m_queue.pop();
    m_mtx.unlock();
    return result;
}
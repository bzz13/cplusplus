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
    server_raft_receiver(const replica& self)
        : m_started(false), m_self(self), m_acceptor(self)
    {
    }

    ~server_raft_receiver()
    {
        if(m_handler.joinable())
            m_handler.join();
    }

    void startRequestReciving()
    {
        if (!m_handler.joinable())
        {
            m_handler = std::thread([this](){
                m_started = m_acceptor.start();

                while (m_started)
                {
                    auto stream = m_acceptor.accept();
                    if (stream) {
                        ssize_t total = 0, lenght = 0;
                        char request[4096];
                        while( total < 4096 && (lenght = stream->receive(request + total, sizeof(request) - total, 50)) > 0)
                            total += lenght;
                        if (total > 0)
                        {
                            request[total] = 0;
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

    void stopRequestReciving()
    {
        m_mtx.lock();
        m_started = false;
        while(!m_queue.empty())
            m_queue.pop();
        m_mtx.unlock();
    }

    bool hasRequest()
    {
        bool result;
        m_mtx.lock();
        result = !m_queue.empty();
        m_mtx.unlock();
        return result;
    }

    p getRequest()
    {
        p result;
        m_mtx.lock();
        if (!m_queue.empty())
        {
            result = m_queue.front();
            m_queue.pop();
        }
        m_mtx.unlock();
        return result;
    }
};
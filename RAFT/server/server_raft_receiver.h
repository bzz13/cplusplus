#pragma once

#include <functional>
#include <queue>
#include <mutex>
#include <memory>
#include <utility> 
#include <thread>
#include "../replicas/replica.h"
#include "../tcp/tcpacceptor.h"


template<typename TK, typename TV>
class server_raft_receiver
{
    typedef std::shared_ptr<TCPStream> spt_strm;
    typedef std::unique_lock<std::mutex> lock;

    bool                                m_started;
    std::mutex                          m_mtx;
    std::thread                         m_handler;
    std::thread                         m_proccer;
    TCPAcceptor                         m_acceptor;
    std::queue<spt_strm>                m_queue;

public:
    server_raft_receiver(const replica& self)
        : m_started(false), m_acceptor(self)
    {
    }

    ~server_raft_receiver()
    {
        if(m_handler.joinable()) m_handler.join();
        if(m_proccer.joinable()) m_proccer.join();
    }

    void startRequestReciving(std::function<void (spt_strm&)> worker)
    {
        m_started = m_acceptor.start();
        if (!m_handler.joinable())
        {
            m_handler = std::thread([this](){
                while (m_started)
                {
                    auto stream = m_acceptor.accept();
                    std::clog << "acepted" << std::endl;
                    lock lk(m_mtx);
                    if (m_started)
                        m_queue.push(stream);
                    std::clog << "receiving m_queue.size: " << m_queue.size() << std::endl;
                }
            });
        }

        if (!m_proccer.joinable())
        {
            m_proccer = std::thread([this, worker](){
                while (m_started)
                {
                    lock lk(m_mtx);
                    if (!m_queue.empty())
                    {
                        auto stream = m_queue.front();
                        m_queue.pop();
                        try
                        {
                            worker(stream);
                            m_queue.push(stream);
                            std::clog << "receiving m_queue.size: " << m_queue.size() << std::endl;
                        }
                        catch(TCPException& tcpe)
                        {
                            std::cerr << tcpe.what() << std::endl;
                        }
                    }
                }
            });
        }
    }

    void stopRequestReciving()
    {
        lock lk(m_mtx);
        m_started = false;
        while(!m_queue.empty())
            m_queue.pop();
    }
};
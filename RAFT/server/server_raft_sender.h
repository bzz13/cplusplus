#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <memory>
#include <utility> 
#include <thread>
#include "../replicas/replica.h"
#include "../tcp/tcpconnector.h"

class server_raft_sender
{
    typedef std::pair<const replica, const std::string> p;

    bool                            m_started;
    replica                         m_self;
    std::mutex                      m_mtx;
    std::thread                     m_handler;
    TCPConnector                    m_connnector;
    std::queue<p>                   m_queue;

public:
    server_raft_sender(const replica& self)
        : m_started(false), m_self(self)
    {
    }

    ~server_raft_sender()
    {
        if(m_handler.joinable())
            m_handler.join();
    }

    void sendRequest(const replica& r, const std::string& message)
    {
        m_mtx.lock();
        m_queue.push(std::make_pair(r, message));
        m_mtx.unlock();
    }

    void sendRequest(replicas& rs, const std::string& message)
    {
        m_mtx.lock();
        for (auto r: rs)
            m_queue.push(std::make_pair(r, message));
        m_mtx.unlock();
    }

    void sendRequest(replicas& rs, const replica& exclude, const std::string& message)
    {
        m_mtx.lock();
        for (auto r: rs)
            if (r == exclude)
                continue;
            else
                m_queue.push(std::make_pair(r, message));
        m_mtx.unlock();
    }

    void startRequestSending()
    {
        m_started = true;
        if (!m_handler.joinable())
        {
            m_handler = std::thread([this](){
                while (m_started)
                {
                    m_mtx.lock();
                    if (!m_queue.empty())
                    {
                        auto p = m_queue.front();
                        m_queue.pop();
                        m_mtx.unlock();
                        try
                        {
                            auto stream = m_connnector.connect(p.first);
                            if (stream && m_started)
                            {
                                std::clog << ">>> to: " << p.first << " msg: " << p.second << std::endl;
                                stream->send(p.second);
                            }
                        }
                        catch(TCPException& tcpe)
                        {
                            std::cerr << tcpe.what() << std::endl;
                        }
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

    void stopRequestSending()
    {
        m_mtx.lock();
        m_started = false;
        while(!m_queue.empty())
            m_queue.pop();
        m_mtx.unlock();
    }
};
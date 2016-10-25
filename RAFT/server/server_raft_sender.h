#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <memory>
#include <utility>
#include <unordered_map>
#include <thread>
#include "../replicas/replica.h"
#include "../tcp/tcpconnector.h"

class server_raft_sender
{
    typedef std::pair<const replica, const std::string> p;
    typedef std::shared_ptr<TCPStream> spt_strm;
    typedef thread_safe_queue<p> p_queue;

    bool                                        m_started;
    replica                                     m_self;
    std::mutex                                  m_mtx;
    std::thread                                 m_handler;
    TCPConnector                                m_connnector;
    p_queue                                     m_queue;
    std::unordered_map<std::string, spt_strm>   m_map;

    void start()
    {
        m_started = true;
        if (!m_handler.joinable())
        {
            m_handler = std::thread([this](){
                while (m_started)
                {
                    auto front_p = m_queue.try_pop();
                    if (front_p.first)
                    {
                        auto p = front_p.second;
                        try
                        {
                            auto stream = m_map[p.first.toString()];
                            if (!stream)
                            {
                                stream = m_connnector.connect(p.first, 10);
                                m_map[p.first.toString()] = stream;
                            }
                            if (stream && m_started)
                            {
                                stream << p.second;
                                std::cout << ">>> to: " << p.first << " msg: " << p.second << std::endl;
                            }
                        }
                        catch(TCPException& tcpe)
                        {
                            m_map.erase(p.first.toString());
                            std::cout << tcpe.what() << std::endl;
                        }
                    }
                    std::this_thread::yield();
                }
            });
        }
    }

public:
    server_raft_sender(const replica& self)
        : m_started(false), m_self(self)
    {
        start();
    }

    ~server_raft_sender()
    {
        if(m_handler.joinable())
            m_handler.join();
    }

    void send_request(const replica& r, const std::string& message)
    {
        m_queue.push(std::make_pair(r, message));
    }


    void stop()
    {
        m_started = false;
    }
};
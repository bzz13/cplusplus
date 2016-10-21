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
    typedef std::unique_lock<std::mutex> lock;

    bool                                        m_started;
    replica                                     m_self;
    std::mutex                                  m_mtx;
    std::thread                                 m_handler;
    TCPConnector                                m_connnector;
    std::queue<p>                               m_queue;
    std::unordered_map<std::string, spt_strm>   m_map;

public:
    server_raft_sender(const replica& self)
        : m_started(false), m_self(self)
    {
    }

    ~server_raft_sender()
    {
        if(m_handler.joinable()) m_handler.join();
    }

    void sendRequest(const replica& r, const std::string& message)
    {
        lock lk(m_mtx);
        m_queue.push(std::make_pair(r, message));
        std::clog << "sending m_queue.size: " << m_queue.size() << std::endl;
    }

    void sendRequest(replicas& rs, const std::string& message)
    {
        lock lk(m_mtx);
        for (auto r: rs)
            m_queue.push(std::make_pair(r, message));
        std::clog << "sending m_queue.size: " << m_queue.size() << std::endl;
    }

    void sendRequest(replicas& rs, const replica& exclude, const std::string& message)
    {
        lock lk(m_mtx);
        for (auto r: rs)
            if (!(r == exclude))
                m_queue.push(std::make_pair(r, message));
        std::clog << "sending m_queue.size: " << m_queue.size() << std::endl;
    }

    void startRequestSending()
    {
        m_started = true;
        if (!m_handler.joinable())
        {
            m_handler = std::thread([this](){
                while (m_started)
                {
                    lock lk(m_mtx);
                    if (!m_queue.empty())
                    {
                        auto p = m_queue.front();
                        m_queue.pop();
                        lk.unlock();
                        std::clog << "sending m_queue.size: " << m_queue.size() << std::endl;
                        try
                        {
                            auto stream = m_map[p.first.toString()];
                            if (!stream)
                            {
                                stream = m_connnector.connect(p.first);
                                m_map[p.first.toString()] = stream;
                            }
                            if (stream && m_started)
                            {
                                stream << p.second;
                                std::clog << ">>> to: " << p.first << " msg: " << p.second << std::endl;
                            }
                        }
                        catch(TCPException& tcpe)
                        {
                            m_map.erase(p.first.toString());
                            std::cerr << tcpe.what() << std::endl;
                        }
                    }
                }
            });
        }
    }

    void stopRequestSending()
    {
        lock lk(m_mtx);
        m_started = false;
        while(!m_queue.empty())
            m_queue.pop();
    }
};
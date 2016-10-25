#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <utility> 
#include <thread>
#include "../replicas/replica.h"
#include "../tcp/tcpacceptor.h"
#include "../thread_safe_queue.h"


template<typename TK, typename TV>
class server_raft_receiver
{
    typedef std::shared_ptr<TCPStream> spt_strm;
    typedef thread_safe_queue<spt_strm> ts_queue;

    bool                                m_started;
    std::thread                         m_handler;
    std::thread                         m_reader;
    TCPAcceptor                         m_acceptor;
    ts_queue                            m_queue;

public:
    server_raft_receiver(const replica& self)
        : m_started(false), m_acceptor(self)
    {
    }

    ~server_raft_receiver()
    {
        if(m_handler.joinable()) m_handler.join();
        if(m_reader.joinable()) m_reader.join();
    }

    void startRequestReciving(std::function<void (spt_strm&)> streamHandler)
    {
        m_started = m_acceptor.start();

        if (!m_handler.joinable())
        {
            m_handler = std::thread([this](){
                while (m_started)
                {
                    auto stream = m_acceptor.accept();
                    std::cout << "accepted" << std::endl;
                    m_queue.push(stream);
                    std::this_thread::yield();
                }
            });
        }

        if (!m_reader.joinable())
        {
            m_reader = std::thread([this, streamHandler](){
                while (m_started)
                {
                    auto front_p = m_queue.try_pop();
                    if (front_p.first)
                    {
                        auto stream = front_p.second;
                        try
                        {
                            streamHandler(stream);
                            m_queue.push(stream);
                        }
                        catch(TCPTimeoutException& tcptoe)
                        {
                            // std::cout << "not ready yet" << std::endl;
                            m_queue.push(stream);
                        }
                        catch(TCPException& tcpe)
                        {
                            std::cout << tcpe.what() << std::endl;
                        }
                    }
                    std::this_thread::yield();
                }
            });
        }
    }

    void stopRequestReciving()
    {
        m_started = false;
    }
};
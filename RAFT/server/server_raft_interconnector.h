#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "server_raft_receiver.h"
#include "server_raft_sender.h"
#include "../replicas/replica.h"
#include "../rpc/server_proto_operation.h"
#include "../rpc/server_proto_parser.h"
#include "../tcp/tcpstream.h"

template<typename TK, typename TV>
class server_raft_interconnector
{
    typedef server_proto_operation<TK, TV> spo;
    typedef std::shared_ptr<spo> spt_spo;

    server_raft_receiver<TK, TV>    m_receiver;
    server_raft_sender              m_sender;
    server_proto_parser<TK, TV>     m_parser;

public:
    server_raft_interconnector(const replica& self)
        :m_receiver(self), m_sender(self)
    {
    }

    void send_messages(std::vector<std::pair<const replica, const std::string>> msgs)
    {
        for(auto p: msgs)
            m_sender.send_request(p.first, p.second);
    }

    void send_messages(replicas& rs, const std::string& message)
    {
        for(auto r: rs)
            m_sender.send_request(r, message);
    }

    void send_messages(replicas& rs, replica& exclude, const std::string& message)
    {
        for(auto r: rs)
            if (r != exclude)
                m_sender.send_request(r, message);
    }

    void send_message(replica& r, const std::string& message)
    {
        m_sender.send_request(r, message);
    }

    void send_message(std::shared_ptr<TCPStream> stream, const std::string& message)
    {
        m_sender.send_request(stream, message);
    }

    std::pair<bool, spt_spo> try_get_message()
    {
        auto p = m_receiver.try_get_stream();
        if (p.first)
        {
            auto stream = p.second;
            if (stream)
            {
                try
                {
                    auto operation = m_parser.parse(stream);
                    m_receiver.delay_stream(stream);
                    return std::make_pair(true, operation);
                }
                catch(TCPTimeoutException& tcptoe)
                {
                    // std::cout << "not ready yet" << std::endl;
                    m_receiver.delay_stream(stream);
                }
                catch(TCPException& tcpe)
                {
                    std::cout << tcpe.what() << std::endl;
                }
            }
        }
        return std::make_pair(false, nullptr);
    }

    void stop()
    {
        m_receiver.stop();
        m_sender.stop();
    }
};
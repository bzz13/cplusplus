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
    replica                         m_self;
    server_raft_receiver<TK, TV>    m_receiver;
    server_raft_sender              m_sender;
    server_proto_parser<TK, TV>     m_parser;

public:
    server_raft_interconnector(const replica& self): m_self(self), m_receiver(self), m_sender(self)
    {
    }

    void send_messages(std::vector<std::pair<const replica, const std::string>> msgs)
    {
        for(auto p: msgs)
            m_sender.sendRequest(p.first, p.second);
    }

    std::pair<bool, std::shared_ptr<server_proto_operation<TK, TV>>> try_get_message(const int timeout = 0)
    {
        auto p = m_receiver.try_get_stream();
        if (p.first)
        {
            auto stream = p.second;
            if (stream)
            {
                try{
                    std::string request;
                    stream >> request;
                    std::cout << "<<< " << request << std::endl;
                    server_proto_parser<TK, TV> parser;
                    auto operation = parser.parse(request, stream);
                    m_receiver.delay_stream_later(stream);
                    return std::make_pair(true, operation);
                }
                catch(TCPTimeoutException& tcptoe)
                {
                    // std::cout << "not ready yet" << std::endl;
                    m_receiver.delay_stream_later(stream);
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
#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <unordered_map>
#include "../tcp/tcpconnector.h"

class server_raft_sender
{
    typedef std::shared_ptr<TCPStream> spt_strm;

    TCPConnector                                m_connector;
    std::unordered_map<std::string, spt_strm>   m_map;

public:
    void send_request(const replica& r, const std::string& message)
    {
        try
        {
            auto stream = m_map[r.toString()];
            if (!stream)
            {
                stream = m_connector.connect(r, 10);
                m_map[r.toString()] = stream;
            }
            stream << message;
            std::cout << ">>> to: " << r << " msg: " << message << std::endl;
        }
        catch(TCPException& tcpe)
        {
            if (m_map.find(r.toString()) != m_map.end())
                m_map.erase(r.toString());
            std::cout << tcpe.what() << std::endl;
        }
        catch(...)
        {
            std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
        }
    }


    void send_request(spt_strm stream, const std::string& message)
    {
        try
        {
            if (stream)
                stream << message;
        }
        catch(...)
        {
            std::cout << "*********************" << std::endl;
        }
    }

};
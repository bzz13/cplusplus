#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include "tcpstream.h"
#include "tcpexception.h"
#include "../replicas/replica.h"

class TCPConnector
{
public:
    std::shared_ptr<TCPStream> connect(const char* server, const int port, const unsigned int timeout = 0)
    {
        std::shared_ptr<TCPSocket> connecting_socket(new TCPSocket(socket(AF_INET, SOCK_STREAM, 0)));
        if (timeout == 0
            ? !connecting_socket->connect(server, port)
            : !connecting_socket->connect(server, port, timeout))
        {
            std::stringstream what;
            what << "connect() to " << server << ": " << port << (timeout == 0 ? " failed" : " with timeout failed");
            throw TCPException(what.str());
        }
        return std::shared_ptr<TCPStream>(new TCPStream(connecting_socket));
    }

    std::shared_ptr<TCPStream> connect(const std::string& server, const int port, const unsigned int timeout = 0)
    {
        return connect(server.c_str(), port, timeout);
    }

    std::shared_ptr<TCPStream> connect(const replica& server, const unsigned int timeout = 0)
    {
        return connect(server.host().c_str(), server.port(), timeout);
    }
};
#pragma once

#include "tcpsocket.h"
#include <memory>
#include <string>

class TCPStream
{
    std::shared_ptr<TCPSocket> m_socket;

public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    ssize_t send(const char* buffer, size_t length);
    ssize_t send(const std::string& message);
    ssize_t receive(char* buffer, size_t length, unsigned int timeout_ms = 0);

private:
    TCPStream(TCPSocket* socket);
    TCPStream(std::shared_ptr<TCPSocket>& socket);
};
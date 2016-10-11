#pragma once

#include "tcpstream.h"
#include "tcpsocket.h"
#include <string>
#include <memory>

class TCPAcceptor
{
    int                         m_port;
    std::string                 m_address;
    std::unique_ptr<TCPSocket>  m_listning_socket;
    bool                        m_listning = false;

public:
    TCPAcceptor(int port, const std::string& address = "");
    TCPAcceptor(int port, const char* address);

    bool                        start();
    std::unique_ptr<TCPStream>  accept();
};
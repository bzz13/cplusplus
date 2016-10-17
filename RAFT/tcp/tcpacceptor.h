#pragma once

#include "tcpstream.h"
#include "tcpsocket.h"
#include "../replicas/replica.h"
#include <string>
#include <memory>

class TCPAcceptor
{
    int         m_port;
    std::string m_address;
    TCPSocket   m_listning_socket;
    bool        m_listning = false;

public:
	TCPAcceptor(const replica& self);
    TCPAcceptor(int port, const std::string& address = "");
    TCPAcceptor(int port, const char* address);

    bool                        start();
    std::shared_ptr<TCPStream>  accept();
};
#pragma once

#include <memory>
#include <string>
#include <sys/socket.h>
#include "tcpstream.h"
#include "tcpsocket.h"
#include "tcpacceptor.h"
#include "tcpexception.h"
#include "../replicas/replica.h"


class TCPAcceptor
{
    int         m_port;
    std::string m_address;
    TCPSocket   m_listning_socket;
    bool        m_listning = false;

public:
    TCPAcceptor(int port, const std::string& address = "")
        : m_listning_socket(socket(PF_INET, SOCK_STREAM, 0)),
          m_port(port),
          m_address(address)
    {
    }

    TCPAcceptor(const replica& self)
        : TCPAcceptor(self.port(), self.host())
    {
    }

    TCPAcceptor(int port, const char* address)
        : TCPAcceptor(port, std::string(address))
    {
    }

    bool start()
    {
        if (m_listning == true)
            return false;
        if (!m_listning_socket.bind(m_address, m_port))
            throw TCPException("bind() failed");
        if (!m_listning_socket.listen())
            throw TCPException("listen() failed");
        m_listning = true;
        return m_listning;
    }

    std::shared_ptr<TCPStream> accept()
    {
        if (m_listning == false)
            throw TCPException("accept() failed - not listning");
        return std::shared_ptr<TCPStream>(
            new TCPStream(m_listning_socket.accept()));
    }
};
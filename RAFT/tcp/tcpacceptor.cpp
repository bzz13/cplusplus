#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "tcpacceptor.h"

TCPAcceptor::TCPAcceptor(int port, const std::string& address):
    m_listning_socket(socket(PF_INET, SOCK_STREAM, 0)),
    m_port(port),
    m_address(address)
{
}

TCPAcceptor::TCPAcceptor(int port, const char* address)
    : TCPAcceptor(port, std::string(address))
{
}

bool TCPAcceptor::start()
{
    if (m_listning == true)
    {
        return false;
    }

    if (!m_listning_socket.bind(m_port, m_address))
    {
        perror("bind() failed");
        return false;
    }

    if (!m_listning_socket.listen())
    {
        perror("listen() failed");
        return false;
    }
    m_listning = true;
    return m_listning;
}

std::unique_ptr<TCPStream> TCPAcceptor::accept()
{
    if (m_listning == false)
    {
        return nullptr;
    }

    auto accepting_socket = std::unique_ptr<TCPSocket>(
        m_listning_socket.accept());
    if (accepting_socket->raw() < 0)
    {
        perror("accept() failed");
        return nullptr;
    }
    return std::unique_ptr<TCPStream>(new TCPStream(accepting_socket));
}

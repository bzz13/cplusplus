#include "tcpstream.h"

TCPStream::TCPStream(TCPSocket* socket)
    : m_socket(socket)
{
}

TCPStream::TCPStream(std::unique_ptr<TCPSocket>& socket)
    : m_socket(std::move(socket))
{
}

ssize_t TCPStream::send(const char* buffer, size_t length)
{
    return m_socket->send(buffer, length);
}

ssize_t TCPStream::send(const std::string& message)
{
    return m_socket->send(message);
}

ssize_t TCPStream::receive(char* buffer, size_t length, unsigned int timeout)
{
    return m_socket->receive(buffer, length, timeout);
}
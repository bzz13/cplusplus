#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "tcpsocket.h"
#include "tcpexception.h"

class TCPStream
{
    std::shared_ptr<TCPSocket> m_socket;

public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    std::string         m_data;
    const std::string   m_delimetr = "\n";

    TCPStream& operator<<(const std::string& message) throw(TCPException)
    {
        auto length = m_socket->send(message + m_delimetr);
        // std::cout << "TCPStream operator<< result: " << length << std::endl;
        if (length > 0);
            return (*this);
        throw TCPException("cant write message");
    }
    TCPStream& operator<<(const char* message) throw(TCPException)
    {

        return ((*this) << std::string(message));
    }
    TCPStream& operator>>(std::string& message) throw(TCPException)
    {
        int i = 0; while (m_data[i] == '\n') ++i; m_data = m_data.substr(i);

        // std::cout << "before - current socket buffer: \"" << m_data << "\"" << std::endl;
        auto index = m_data.find(m_delimetr[0]);
        if (index != std::string::npos)
        {
            message = m_data.substr(0, index);
            m_data = m_data.substr(index + 1);
            // std::cout << "after - current socket buffer: \"" << m_data << "\"" << std::endl;
            return (*this);
        }
        else
        {
            const size_t size = 1024;
            char buffer[size];
            auto length = m_socket->receive(buffer, size, 10);

            if (length == TCPSocket::TCPStatus::connectionClosed)
                throw TCPException("connectionClosed");
            if (length == TCPSocket::TCPStatus::connectionReset)
                throw TCPException("connectionReset");
            if (length == TCPSocket::TCPStatus::connectionTimedOut)
                throw TCPTimeoutException("connectionTimedOut");

            // std::cout << "received from socket " << length << " bytes" << std::endl;
            buffer[length] = '\0';
            m_data = m_data + std::string(buffer);
            return this->operator>>(message);
        }
    }

    friend std::shared_ptr<TCPStream>& operator<<(std::shared_ptr<TCPStream>& stream, const std::string& message) throw(TCPException);
    friend std::shared_ptr<TCPStream>& operator<<(std::shared_ptr<TCPStream>& stream, const char* message) throw(TCPException);
    friend std::shared_ptr<TCPStream>& operator>>(std::shared_ptr<TCPStream>& stream, std::string& message) throw(TCPException);

private:
    TCPStream(TCPSocket* socket)
        : m_socket(socket)
    {
    }
    TCPStream(std::shared_ptr<TCPSocket>& socket)
        : m_socket(socket)
    {
    }
};

std::shared_ptr<TCPStream>& operator<<(std::shared_ptr<TCPStream>& stream, const std::string& message) throw(TCPException)
{
    (*stream)<<message;
    return stream;
}
std::shared_ptr<TCPStream>& operator<<(std::shared_ptr<TCPStream>& stream, const char* message) throw(TCPException)
{
    (*stream)<<message;
    return stream;
}
std::shared_ptr<TCPStream>& operator>>(std::shared_ptr<TCPStream>& stream, std::string& message) throw(TCPException)
{
    (*stream)>>message;
    return stream;
}
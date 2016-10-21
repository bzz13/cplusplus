#pragma once

#include "tcpsocket.h"
#include "tcpexception.h"
#include <memory>
#include <string>

class TCPStream
{
    std::shared_ptr<TCPSocket> m_socket;

public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    std::string         m_data;
    const std::string   m_delimetr = "\n";

    TCPStream& operator<<(const std::string& message) throw(TCPException);
    TCPStream& operator<<(const char* message) throw(TCPException);
    TCPStream& operator>>(std::string& message) throw(TCPException);

    friend std::shared_ptr<TCPStream>& operator<<(std::shared_ptr<TCPStream>& stream, const std::string& message) throw(TCPException);
    friend std::shared_ptr<TCPStream>& operator<<(std::shared_ptr<TCPStream>& stream, const char* message) throw(TCPException);
    friend std::shared_ptr<TCPStream>& operator>>(std::shared_ptr<TCPStream>& stream, std::string& message) throw(TCPException);

private:
    TCPStream(TCPSocket* socket);
    TCPStream(std::shared_ptr<TCPSocket>& socket);
};
#include "tcpstream.h"

TCPStream::TCPStream(TCPSocket* socket)
    : m_socket(socket)
{
}

TCPStream::TCPStream(std::shared_ptr<TCPSocket>& socket)
    : m_socket(socket)
{
}

TCPStream& TCPStream::operator<<(const std::string& message) throw(TCPException)
{
    if (m_socket->send(message + m_delimetr) >= 0);
        return (*this);
    throw TCPException("cant write message");
}

TCPStream& TCPStream::operator<<(const char* message) throw(TCPException)
{
    return ((*this) << (std::string(message) + m_delimetr));
}

TCPStream& TCPStream::operator>>(std::string& message) throw(TCPException)
{
    auto index = m_data.find(m_delimetr[0]);
    if (index != std::string::npos)
    {
        message = m_data.substr(0, index);
        m_data = m_data.substr(index + 1);
        return (*this);
    }
    else
    {
        const size_t size = 1024;
        char buffer[size];
        auto length = m_socket->receive(buffer, size, 50);

        if (length == TCPSocket::TCPStatus::connectionClosed)
            throw TCPException("connectionClosed");
        if (length == TCPSocket::TCPStatus::connectionReset)
            throw TCPException("connectionReset");
        if (length == TCPSocket::TCPStatus::connectionTimedOut)
            throw TCPTimeoutException("connectionTimedOut");

        buffer[length] = '\0';
        m_data += std::string(buffer);
        return ((*this) >> message);
    }
}

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

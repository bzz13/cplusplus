#ifndef __tcpstream_h__
#define __tcpstream_h__

#include "tcpsocket.h"
#include <memory>
#include <string>
#include <netinet/in.h>

using namespace std;

class TCPStream
{
    unique_ptr<TCPSocket>   m_socket;

public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    ssize_t send(const char* buffer, size_t length);
    ssize_t send(const string& message);
    ssize_t receive(char* buffer, size_t length, unsigned int timeout = 0);

private:
    TCPStream(unique_ptr<TCPSocket>& socket);
    TCPStream() = delete;
    TCPStream(const TCPStream& stream) = delete;
};

#endif

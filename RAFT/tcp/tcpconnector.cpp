#include <memory>
#include <sys/socket.h>

#include "tcpconnector.h"
#include "tcpexception.h"

std::unique_ptr<TCPStream>TCPConnector::connect(const char* server, const int port, const unsigned int timeout)
{
    std::unique_ptr<TCPSocket> connecting_socket(new TCPSocket(socket(AF_INET, SOCK_STREAM, 0)));
    if (timeout == 0
        ? !connecting_socket->connect(server, port)
        : !connecting_socket->connect(server, port, timeout))
        throw TCPException("connect() failed");
    return std::unique_ptr<TCPStream>(new TCPStream(connecting_socket));
}
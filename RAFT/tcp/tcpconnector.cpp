#include <memory>
#include <sys/socket.h>
#include "tcpconnector.h"

std::unique_ptr<TCPStream>TCPConnector::connect(const char* server, const int port, const unsigned int timeout)
{
    std::unique_ptr<TCPSocket> connecting_socket(new TCPSocket(socket(AF_INET, SOCK_STREAM, 0)));
    bool connectionResult = timeout == 0
        ? connecting_socket->connect(server, port)
        : connecting_socket->connect(server, port, timeout);
    return !connectionResult
        ? nullptr
        : std::unique_ptr<TCPStream>(new TCPStream(connecting_socket));
}
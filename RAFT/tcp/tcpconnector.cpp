#include <memory>
#include <sys/socket.h>
#include "tcpsocket.h"
#include "tcpconnector.h"

unique_ptr<TCPStream>TCPConnector::connect(const char* server, const int port, const unsigned int timeout)
{
	unique_ptr<TCPSocket> connecting_socket(new TCPSocket(socket(AF_INET, SOCK_STREAM, 0)));
	struct sockaddr_in address;
	bool connectionResult = timeout == 0
		? connecting_socket->connect(server, port, &address)
		: connecting_socket->connect(server, port, &address, timeout);

	return !connectionResult
		? nullptr
		: unique_ptr<TCPStream>(new TCPStream(connecting_socket, &address));
}
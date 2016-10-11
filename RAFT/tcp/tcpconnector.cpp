#include <memory>
#include <sys/socket.h>
#include "tcpsocket.h"
#include "tcpconnector.h"

unique_ptr<TCPStream> TCPConnector::connect(const char* server, int port)
{
	unique_ptr<TCPSocket> connecting_socket(new TCPSocket(socket(AF_INET, SOCK_STREAM, 0)));
	struct sockaddr_in address;
	return !connecting_socket->connect(server, port, &address)
		? nullptr
		: unique_ptr<TCPStream>(new TCPStream(connecting_socket, &address));
}

unique_ptr<TCPStream>TCPConnector::connect(const char* server, int port, unsigned int timeout)
{
	if (timeout == 0) 
		return connect(server, port);

	unique_ptr<TCPSocket> connecting_socket(new TCPSocket(socket(AF_INET, SOCK_STREAM, 0)));
	struct sockaddr_in address;
	return !connecting_socket->connect(server, port, &address, timeout)
		? nullptr
		: unique_ptr<TCPStream>(new TCPStream(connecting_socket, &address));
}
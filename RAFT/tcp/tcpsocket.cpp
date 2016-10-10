#include <string>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include "tcpsocket.h"


TCPSocket::TCPSocket(int socket): m_socket(socket)
{
}

TCPSocket::~TCPSocket()
{
	close(m_socket);
}


const int TCPSocket::getnative() const
{
	return m_socket;
}

ssize_t TCPSocket::send(const char* buffer, size_t length)
{
	return write(m_socket, buffer, length);
}

ssize_t TCPSocket::send(const std::string& message)
{
	return write(m_socket, message.c_str(), message.size());
}

ssize_t TCPSocket::receive(char* buffer, size_t length, unsigned int timeout)
{
	return timeout == 0
		? read(m_socket, buffer, length)
		: (waitForReadEvent(timeout)
			? read(m_socket, buffer, length)
			: connectionTimedOut);
}

bool TCPSocket::waitForReadEvent(unsigned int timeout)
{
	fd_set sdset;
	struct timeval tv;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	FD_ZERO(&sdset);
	FD_SET(m_socket, &sdset);
	return select(m_socket + 1, &sdset, nullptr, nullptr, &tv) > 0;
}

bool TCPSocket::bind(int m_port, std::string m_address)
{
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = PF_INET;
	address.sin_port = htons(m_port);
	if (m_address.size() > 0)
		inet_pton(PF_INET, m_address.c_str(), &(address.sin_addr));
	else
		address.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	return ::bind(m_socket, (struct sockaddr*)&address, sizeof(address)) == 0;
}

bool TCPSocket::listen()
{
	return ::listen(m_socket, 5) == 0;
}

bool TCPSocket::accept(int& accepted_socket, struct sockaddr_in* address)
{
	socklen_t length = sizeof(*address);
	memset(address, 0, length);
	accepted_socket = ::accept(m_socket, (struct sockaddr*)address, &length);
	return accepted_socket >= 0;
}
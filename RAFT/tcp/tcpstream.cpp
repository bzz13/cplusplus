#include "tcpstream.h"
#include <arpa/inet.h>

TCPStream::TCPStream(unique_ptr<TCPSocket>& socket, struct sockaddr_in* address)
	: m_socket(std::move(socket))
{
	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) & (address->sin_addr.s_addr), ip, sizeof(ip) - 1);
	m_peerIP = ip;
	m_peerPort = ntohs(address->sin_port);
}

// TCPStream::TCPStream(int socket, struct sockaddr_in* address)
// 	: m_socket(new TCPSocket(socket))
// {
// 	char ip[50];
// 	inet_ntop(PF_INET, (struct in_addr*) & (address->sin_addr.s_addr), ip, sizeof(ip) - 1);
// 	m_peerIP = ip;
// 	m_peerPort = ntohs(address->sin_port);
// }

ssize_t TCPStream::send(const char* buffer, size_t length)
{
	return m_socket->send(buffer, length);
}

ssize_t TCPStream::send(const string& message)
{
	return m_socket->send(message);
}

ssize_t TCPStream::receive(char* buffer, size_t length, unsigned int timeout)
{
	return m_socket->receive(buffer, length, timeout);
}

std::string TCPStream::getPeerIP()
{
	return m_peerIP;
}

int TCPStream::getPeerPort()
{
	return m_peerPort;
}
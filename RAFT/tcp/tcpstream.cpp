/*
   TCPStream.h

   TCPStream class definition. TCPStream provides methods to trasnfer
   data between peers over a TCP/IP connection.

   ------------------------------------------

   Copyright © 2013 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include "tcpstream.h"

TCPStream::TCPStream(int sd, struct sockaddr_in* address)
	: m_socket(sd)
{
	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) & (address->sin_addr.s_addr), ip, sizeof(ip) - 1);
	m_peerIP = ip;
	m_peerPort = ntohs(address->sin_port);
}

TCPStream::~TCPStream()
{
	close(m_socket);
}

ssize_t TCPStream::send(const char* buffer, size_t len)
{
	return write(m_socket, buffer, len);
}

ssize_t TCPStream::send(const string& message)
{
	return write(m_socket, message.c_str(), message.size());
}

ssize_t TCPStream::receive(char* buffer, size_t len, unsigned int timeout)
{
	if (timeout == 0) 
		return read(m_socket, buffer, len);

	return
		waitForReadEvent(timeout)
		? read(m_socket, buffer, len)
		: connectionTimedOut;
}

std::string TCPStream::getPeerIP()
{
	return m_peerIP;
}

int TCPStream::getPeerPort()
{
	return m_peerPort;
}

bool TCPStream::waitForReadEvent(unsigned int timeout)
{
	fd_set sdset;
	struct timeval tv;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	FD_ZERO(&sdset);
	FD_SET(m_socket, &sdset);
	return select(m_socket + 1, &sdset, nullptr, nullptr, &tv) > 0;
}

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
#include "tcpstream.h"

TCPStream::TCPStream(int socket, struct sockaddr_in* address)
	: m_socket(socket)
{
	char ip[50];
	inet_ntop(PF_INET, (struct in_addr*) & (address->sin_addr.s_addr), ip, sizeof(ip) - 1);
	m_peerIP = ip;
	m_peerPort = ntohs(address->sin_port);
}

ssize_t TCPStream::send(const char* buffer, size_t length)
{
	return m_socket.send(buffer, length);
}

ssize_t TCPStream::send(const string& message)
{
	return m_socket.send(message);
}

ssize_t TCPStream::receive(char* buffer, size_t length, unsigned int timeout)
{
	return m_socket.receive(buffer, length, timeout);
}

std::string TCPStream::getPeerIP()
{
	return m_peerIP;
}

int TCPStream::getPeerPort()
{
	return m_peerPort;
}
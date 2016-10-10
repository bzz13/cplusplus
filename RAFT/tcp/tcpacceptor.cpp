/*
   TCPAcceptor.cpp

   TCPAcceptor class definition. TCPAcceptor provides methods to passively
   establish TCP/IP connections with clients.

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

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "tcpacceptor.h"

TCPAcceptor::TCPAcceptor(int port, const std::string& address):
	m_listning_socket(socket(PF_INET, SOCK_STREAM, 0)),
	m_port(port),
	m_address(address)
{
}

TCPAcceptor::TCPAcceptor(int port, const char* address)
	: TCPAcceptor(port, std::string(address))
{
}

bool TCPAcceptor::start()
{
	if (m_listning == true)
	{
		return false;
	}

	if (!m_listning_socket.bind(m_port, m_address))
	{
		perror("bind() failed");
		return false;
	}

	if (!m_listning_socket.listen())
	{
		perror("listen() failed");
		return false;
	}
	m_listning = true;
	return m_listning;
}

unique_ptr<TCPStream> TCPAcceptor::accept()
{
	if (m_listning == false)
	{
		return nullptr;
	}

	struct sockaddr_in address;
	socklen_t len = sizeof(address);
	memset(&address, 0, sizeof(address));
	int socket = ::accept(m_listning_socket.getnative(), (struct sockaddr*)&address, &len);
	if (socket < 0)
	{
		perror("accept() failed");
		return nullptr;
	}
	return unique_ptr<TCPStream>(new TCPStream(socket, &address));
}

/*
   TCPStream.h

   TCPStream class interface. TCPStream provides methods to trasnfer
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

#ifndef __tcpstream_h__
#define __tcpstream_h__

#include "tcpsocket.h"
#include <string>
#include <netinet/in.h>

using namespace std;

class TCPStream
{
	TCPSocket	m_socket;
	string		m_peerIP;
	int			m_peerPort;

public:
	friend class TCPAcceptor;
	friend class TCPConnector;

	ssize_t send(const char* buffer, size_t length);
	ssize_t send(const string& message);
	ssize_t receive(char* buffer, size_t length, unsigned int timeout = 0);

	string	getPeerIP();
	int		getPeerPort();

private:
	TCPStream(int socket, struct sockaddr_in* address);
	TCPStream() = delete;
	TCPStream(const TCPStream& stream) = delete;
};

#endif

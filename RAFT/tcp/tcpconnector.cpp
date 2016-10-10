/*
   TCPConnector.h

   TCPConnector class definition. TCPConnector provides methods to actively
   establish TCP/IP connections with a server.

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
   limitations under the License
*/

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <memory>
#include "tcpconnector.h"

unique_ptr<TCPStream> TCPConnector::connect(const char* server, int port)
{
	struct sockaddr_in address;

	memset (&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (resolveHostName(server, &(address.sin_addr)) != 0 )
	{
		inet_pton(PF_INET, server, &(address.sin_addr));
	}

	// Create and connect the socket, bail if we fail in either case
	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_descriptor < 0) {
		perror("socket() failed");
		return nullptr;
	}
	if (::connect(socket_descriptor, (struct sockaddr*)&address, sizeof(address)) != 0) {
		perror("connect() failed");
		close(socket_descriptor);
		return nullptr;
	}
	return unique_ptr<TCPStream>(new TCPStream(socket_descriptor, &address));
}

unique_ptr<TCPStream>TCPConnector::connect(const char* server, int port, unsigned int timeout)
{
	if (timeout == 0) return connect(server, port);

	struct sockaddr_in address;

	memset (&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (resolveHostName(server, &(address.sin_addr)) != 0 )
	{
		inet_pton(PF_INET, server, &(address.sin_addr));
	}

	long arg;
	fd_set sdset;
	struct timeval tv;
	socklen_t len;
	int result = -1, valopt, socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	// Bail if we fail to create the socket
	if (socket_descriptor < 0) {
		perror("socket() failed");
		return nullptr;
	}

	// Set socket to non-blocking
	arg = fcntl(socket_descriptor, F_GETFL, nullptr);
	arg |= O_NONBLOCK;
	fcntl(socket_descriptor, F_SETFL, arg);

	// Connect with time limit
	string message;
	if ((result = ::connect(socket_descriptor, (struct sockaddr *)&address, sizeof(address))) < 0)
	{
		if (errno == EINPROGRESS)
		{
			tv.tv_sec = timeout;
			tv.tv_usec = 0;
			FD_ZERO(&sdset);
			FD_SET(socket_descriptor, &sdset);
			int s = -1;
			do
			{
				s = select(socket_descriptor + 1, nullptr, &sdset, nullptr, &tv);
			} while (s == -1 && errno == EINTR);
			if (s > 0)
			{
				len = sizeof(int);
				getsockopt(socket_descriptor, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &len);
				if (valopt)
				{
					fprintf(stderr, "connect() error %d - %s\n", valopt, strerror(valopt));
				}
				// connection established
				else
				{
					result = 0;
				}
			}
			else
			{
				fprintf(stderr, "connect() timed out\n");
			}
		}
		else
		{
			fprintf(stderr, "connect() error %d - %s\n", errno, strerror(errno));
		}
	}

	// Return socket to blocking mode
	arg = fcntl(socket_descriptor, F_GETFL, nullptr);
	arg &= (~O_NONBLOCK);
	fcntl(socket_descriptor, F_SETFL, arg);

	// Create stream object if connected
	return (result == -1)
		? nullptr
		: unique_ptr<TCPStream>(new TCPStream(socket_descriptor, &address));
}

int TCPConnector::resolveHostName(const char* hostname, struct in_addr* addr)
{
	struct addrinfo *res;

	int result = getaddrinfo(hostname, nullptr, nullptr, &res);
	if (result == 0)
	{
		memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
		freeaddrinfo(res);
	}
	return result;
}

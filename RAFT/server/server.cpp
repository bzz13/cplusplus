/*
   server.cpp

   Test server for the tcpsockets classes which handles connections
   iteratively.

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

#include <stdlib.h>
#include <iostream>
#include <memory>
#include "../tcp/tcpacceptor.h"

int main(int argc, char** argv)
{
	if (argc < 2 || argc > 4) {
		cerr << "usage: server <port> [<ip>]" << endl;
		return 1;
	}

	unique_ptr<TCPAcceptor> acceptor(
		argc == 3 
			? new TCPAcceptor(atoi(argv[1]), argv[2])
			: new TCPAcceptor(atoi(argv[1]))
	);
	if (acceptor->start()) {
		while (1) {
			auto stream = acceptor->accept();
			if (stream) {
				ssize_t len;
				char line[256];
				while ((len = stream->receive(line, sizeof(line))) > 0) {
					line[len] = 0;
					clog << "received - " << line << endl;
					stream->send(line, len);
				}
			}
		}
	}
	return 0;
}


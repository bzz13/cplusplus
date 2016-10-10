/*
   client.cpp

   Test client for the tcpsockets classes.

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
#include <string>
#include <memory>
#include <iostream>
#include "../tcp/tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 3) {
		cerr << "usage: " << argv[0] << " <port> <ip>" << endl;
		return 1;
	}

	int len;
	char line[256];
	TCPConnector connector;

	auto stream = connector.connect(argv[2], atoi(argv[1]));
	if (stream) {
		string message("Is there life on Mars?");
		stream->send(message);
		clog << "sent - " << message << endl;
		len = stream->receive(line, sizeof(line));
		line[len] = 0;
		clog << "received - " << line << endl;
	}

	stream = connector.connect(argv[2], atoi(argv[1]));
	if (stream) {
		string message("Why is there air?");
		stream->send(message);
		clog << "sent - " << message << endl;
		len = stream->receive(line, sizeof(line));
		line[len] = 0;
		clog << "received - " << line << endl;
	}
	return 0;
}

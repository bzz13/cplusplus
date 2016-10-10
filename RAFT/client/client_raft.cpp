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

	cout << "input request" << endl;
	while(true)
	{
		auto stream = connector.connect(argv[2], atoi(argv[1]));
		if (stream) {
			string message;
			getline(cin, message);

			if (message == "exit")
				break;

			stream->send(message);
			len = stream->receive(line, sizeof(line));
			line[len] = 0;
			clog << "received: " << line << endl;

			if (message == "stop")
				break;
		}
	}
	return 0;
}

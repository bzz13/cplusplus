#include <stdlib.h>
#include <string>
#include <memory>
#include <iostream>
#include "../tcp/tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cerr << "usage: " << argv[0] << "<ip> <port>" << endl;
        return 1;
    }

    int len;
    char line[256];

    cout << "input request" << endl;
    while (true)
    {
        string message;
        getline(cin, message);

        if (message == "exit")
            break;

        TCPConnector connector;
        auto stream = connector.connect(argv[1], atoi(argv[2]));
        if (stream) {
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

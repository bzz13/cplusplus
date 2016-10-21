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

    TCPConnector connector;
    auto stream = connector.connect(argv[1], atoi(argv[2]));

    cout << "input request" << endl;
    while (true)
    {
        string request;
        string response;
        getline(cin, request);

        if (request == "exit")
            break;

        if (stream) {
            stream << request;
            stream >> response;
            clog << "received: " << response << endl;
            if (request == "stop")
                break;
        }
    }
    return 0;
}

#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <thread>
#include "../tcp/tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "usage: " << argv[0] << "<ip> <port>" << endl;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            stream >> response;
            cout << "received: " << response << endl;
            if (request == "stop")
                break;
        }
    }
    return 0;
}

#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <thread>
#include "../tcp/tcpstream.h"
#include "../tcp/tcpconnector.h"

using namespace std;

string getResult(std::shared_ptr<TCPStream>& stream)
{
    string response;
    while(true)
    {
        try
        {
            stream >> response;
            cout << "received: \"" << response << "\"" << endl;
            break;
        }
        catch(TCPTimeoutException& tcptoe)
        {
        }
    }
    return response;
}

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
        getline(cin, request);

        if (request == "exit")
            break;

        if (stream) {
            stream << request;
            auto response = getResult(stream);
            if (request == "stop")
                break;
        }
    }
    return 0;
}

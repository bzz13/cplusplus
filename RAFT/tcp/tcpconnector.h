#pragma once

#include "tcpstream.h"
#include <memory>
#include <string>

class TCPConnector
{
public:
    std::unique_ptr<TCPStream> connect(const char* server, const int port, const unsigned int timeout = 0);
    std::unique_ptr<TCPStream> connect(const std::string& server, const int port, const unsigned int timeout = 0);
};
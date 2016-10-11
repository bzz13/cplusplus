#pragma once

#include "tcpstream.h"
#include <memory>

class TCPConnector
{
public:
    std::unique_ptr<TCPStream> connect(const char* server, const int port, const unsigned int timeout = 0);
};
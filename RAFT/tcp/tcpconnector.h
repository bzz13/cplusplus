#pragma once

#include "tcpstream.h"
#include "../replicas/replica.h"
#include <memory>
#include <string>

class TCPConnector
{
public:
    std::shared_ptr<TCPStream> connect(const char* server, const int port, const unsigned int timeout = 0);
    std::shared_ptr<TCPStream> connect(const std::string& server, const int port, const unsigned int timeout = 0);
    std::shared_ptr<TCPStream> connect(const replica& server, const unsigned int timeout = 0);
};
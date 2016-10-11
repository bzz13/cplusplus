#ifndef __tcpconnector_h__
#define __tcpconnector_h__

#include "tcpstream.h"
#include <memory>

class TCPConnector
{
public:
	unique_ptr<TCPStream> connect(const char* server, const int port, const unsigned int timeout = 0);
};

#endif

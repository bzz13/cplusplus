#ifndef __tcpconnector_h__
#define __tcpconnector_h__

#include "tcpstream.h"
#include <memory>

class TCPConnector
{
public:
	unique_ptr<TCPStream> connect(const char* server, int port);
	unique_ptr<TCPStream> connect(const char* server, int port, unsigned int timeout);
};

#endif

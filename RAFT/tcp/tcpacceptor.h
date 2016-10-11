#ifndef __tcpacceptor_h__
#define __tcpacceptor_h__

#include "tcpstream.h"
#include "tcpsocket.h"
#include <string>
#include <memory>

class TCPAcceptor
{
	int						m_port;
	string					m_address;
	unique_ptr<TCPSocket>	m_listning_socket;
	bool					m_listning = false;

public:
	TCPAcceptor(int port, const std::string& address = "");
	TCPAcceptor(int port, const char* address);

	bool					start();
	unique_ptr<TCPStream>	accept();
};

#endif

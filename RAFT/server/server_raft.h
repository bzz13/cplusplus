#ifndef server_raft_h
#define server_raft_h

#include <string>
#include <memory>
#include <sstream>
#include <type_traits>
#include "../store.h"
#include "../tcp/tcpacceptor.h"

using namespace std;

template<typename TK, typename TV>
class server_raft
{
	int m_port;
	bool m_started;
	store<TK,TV> m_store;

	string handleRequest(string request);

public:
	server_raft(int port, string logpath, bool restore = false);

	void start();
};

template<typename TK, typename TV>
server_raft<TK, TV>::server_raft(int port, string logpath, bool restore)
	: m_port(port), m_started(false), m_store(logpath, restore)
{
	if (restore)
		m_store.showStore();
}

template<typename TK, typename TV>
void server_raft<TK, TV>::start()
{
	if (m_started) return;

	TCPAcceptor acceptor(m_port);
	if (acceptor.start())
	{
		m_started = true;
		while(m_started)
		{
			auto stream = acceptor.accept();
			if (stream) {
				ssize_t len;
				char line[256];
				while ((len = stream->receive(line, sizeof(line))) > 0) {
					line[len] = 0;
					string response = handleRequest(string(line));
					stream->send(response);
				}
			}
		}
	}
}

template<typename TK, typename TV>
string server_raft<TK, TV>::handleRequest(string request)
{
	clog << request;
	stringstream ss(request);
	string method;
	ss >> method;
	if (method == "stop")
	{
		m_started = false;
		clog << " -> stop" << endl;
		return string("stopped");
	}

	TK key;
	ss >> key;
	if (method == "get")
	{
		stringstream respstream;
		respstream << m_store.get(key);
		clog << " -> " << respstream.str() << endl;
		return respstream.str();
	}
	if (method == "del")
	{
		m_store.del(key);
		clog << " -> deleted" << endl;
		return string("deleted");
	}
	if (method == "set")
	{
		TV val;
		ss >> val;
		m_store.set(key, val);
		stringstream respstream;
		respstream << m_store.get(key);
		clog << " -> " << respstream.str() << endl;
		return respstream.str();
	}

	return string("undef request");
}

#endif
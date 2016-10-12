#ifndef server_raft_h
#define server_raft_h

#include <string>
#include <memory>
#include <sstream>
#include <type_traits>
#include <thread>
#include <chrono>
#include "../replicas/replicas.h"
#include "../store.h"
#include "../tcp/tcpacceptor.h"
#include "../tcp/tcpconnector.h"
#include "../tcp/tcpexception.h"

using namespace std;

template<typename TK, typename TV>
class server_raft
{
    int             m_term;
    int             m_status = serverStatus::followeer;
    int             m_port;
    bool            m_started;
    replicas        m_replicas;
    store<TK, TV>   m_store;

    std::thread     heartBeat;
    std::thread     handler;

    string handleRequest(string request);

public:
    enum serverStatus
    {
        followeer = 0,
        candidate = 1,
        leader = 2,
    };

    server_raft(int port, int status, string replicaspath, string logpath, bool restore = false);
    ~server_raft();

    void start();
    void startHeartBeatSending();
    void startRequstHandling();
};

template<typename TK, typename TV>
server_raft<TK, TV>::server_raft(int port, int status, string replicaspath, string logpath, bool restore)
    : m_term(1), m_port(port), m_status(status), m_started(false),
      m_replicas(replicaspath), m_store(logpath, restore)
{
    if (restore)
        m_store.showStore();
}

template<typename TK, typename TV>
server_raft<TK, TV>::~server_raft()
{
    if (heartBeat.joinable()) heartBeat.join();
    if (handler.joinable()) handler.join();
}

template<typename TK, typename TV>
void server_raft<TK, TV>::start()
{
    std::cout << "current term " << m_term << std::endl;
    if (m_started)
        return;

    m_started = true;
    startRequstHandling();
    if (m_status == serverStatus::leader)
        startHeartBeatSending();
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

    if (method == "heartBeat")
    {
        int leader_term; int leader_port;
        ss >> leader_term >> leader_port;

        stringstream respstream;
        if (m_term == leader_term)
        {
            respstream << leader_term << " " << leader_port;
        }
        else
        {
            m_term ++;
            m_status = serverStatus::candidate;
            respstream << m_term << " " << m_port;
        }
        clog << " -> " << respstream.str() << endl;
        return respstream.str();
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

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatSending()
{
    heartBeat = std::thread([this](){
        while(m_started && m_status == serverStatus::leader)
        {
            TCPConnector connector;
            for(auto repl: m_replicas)
            {
                if (repl.port() == m_port)
                    continue;
                try
                {
                    auto stream = connector.connect(repl.host(), repl.port());
                    if (stream)
                    {
                        stringstream message;
                        message << "heartBeat " << m_term << " " << m_port;
                        std::clog << message.str() << std::endl;
                        stream->send(message.str());
                        char resp[256];
                        stream->receive(resp, 256);
                        std::clog << resp << std::endl;
                    }
                }
                catch(TCPException& tcpe)
                {
                    std::cerr << "hb " << tcpe.what() << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startRequstHandling()
{
    handler = std::thread([this](){
        TCPAcceptor acceptor(m_port);
        m_started = acceptor.start();
        while (m_started)
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
    });
}

#endif
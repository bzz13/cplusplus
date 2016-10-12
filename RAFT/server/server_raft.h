#ifndef server_raft_h
#define server_raft_h

#include <string>
#include <memory>
#include <sstream>
#include <type_traits>
#include <thread>
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

    string handleRequest(const std::string&  request);
    std::vector<std::string> sendForAll(const std::string& message);
    bool isMajority(const std::vector<string>& results, TV val);
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
std::string server_raft<TK, TV>::handleRequest(const std::string& request)
{
    clog << request;
    stringstream ss(request);
    string method;
    ss >> method;
    if (method == "stop")
    {
        m_started = false;
        clog << " -> stop" << endl;
        return "stopped";
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
        return "deleted";
    }
    if (method == "set")
    {
        TV val;
        ss >> val;

        if((m_status == serverStatus::leader && isMajority(sendForAll(request), val)) ||
           (m_status == serverStatus::followeer))
        {
            m_store.set(key, val);
            stringstream respstream;
            respstream << m_store.get(key);
            clog << " -> " << respstream.str() << endl;
            return respstream.str();
        }
        return "not applied";
    }

    return "undef request";
}

template<typename TK, typename TV>
std::vector<string> server_raft<TK, TV>::sendForAll(const std::string& message)
{
    std::vector<std::string> results;
    TCPConnector connector;
    std::clog << "begin replication: " << std::endl;
    for(auto repl: m_replicas)
    {
        if (repl.port() == m_port)
            continue;
        try
        {
            auto stream = connector.connect(repl.host(), repl.port());
            if (stream)
            {
                std::clog << message << std::endl;
                stream->send(message);
                char resp[256];
                auto recieved = stream->receive(resp, 256);
                resp[recieved] = 0;
                results.push_back(std::string(resp));
            }
        }
        catch(TCPException& tcpe)
        {
            results.push_back("");
        }
    }
    std::clog << "end replication" << std::endl;
    return results;
}

template<typename TK, typename TV>
bool server_raft<TK, TV>::isMajority(const std::vector<string>& results, TV val)
{
    std::stringstream s;
    s << val;
    int ac = 0;
    for(auto res: results)
        if (res == s.str()) ++ac;
    return ac > results.size() / 2;
}

template<typename TK, typename TV>
void server_raft<TK, TV>::startHeartBeatSending()
{
    heartBeat = std::thread([this](){
        while(m_started && m_status == serverStatus::leader)
        { 
            stringstream message;
            message << "heartBeat " << m_term << " " << m_port;
            std::vector<string> resps(sendForAll(message.str()));
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
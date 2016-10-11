#pragma once

#include <string>
#include <memory>

class TCPSocket
{
    int m_socket;

    bool waitForReadEvent(const unsigned int& timeout);
    bool resolveHostName(const char* hostname, struct in_addr* addr);
    void setBlocking();
    void setNonBlocking();
public:
    enum {
        connectionClosed = 0,
        connectionReset = -1,
        connectionTimedOut = -2
    };

    explicit TCPSocket(const int& socket);
    ~TCPSocket();

    const int raw() const;

    ssize_t send(const char* buffer, size_t length);
    ssize_t send(const std::string& message);
    ssize_t receive(char* buffer, size_t length, unsigned int timeout = 0);

    bool connect(const char* hostname, const int& port);
    bool connect(const char* hostname, const int& port, const unsigned int& timeout);

    bool bind(const std::string& hostname, const int& port);
    bool listen();
    TCPSocket* accept();
};
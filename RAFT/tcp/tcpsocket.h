#pragma once

#include <string>
#include <memory>

class TCPSocket
{
    friend class TCPAcceptor;

    int m_socket;

    bool waitForReadEvent(unsigned int timeout);

    bool bind(int m_port, std::string m_address);
    bool listen();
    TCPSocket accept();

    bool resolveHostName(const char* hostname, struct in_addr* addr);
    void setBlocking();
    void setNonBlocking();
public:
    enum {
        connectionClosed = 0,
        connectionReset = -1,
        connectionTimedOut = -2
    };

    explicit TCPSocket(int socket);
    ~TCPSocket();

    const int getnative() const;

    ssize_t send(const char* buffer, size_t length);
    ssize_t send(const std::string& message);
    ssize_t receive(char* buffer, size_t length, unsigned int timeout = 0);

    bool connect(const char* hostname, int port);
    bool connect(const char* hostname, int port, unsigned int timeout);
};
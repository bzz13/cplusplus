#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include "tcpexception.h"

class TCPSocket
{
    int m_socket;

    bool waitForReadEvent(const unsigned int timeout_ms)
    {
        fd_set sdset;
        struct timeval tv;

        tv.tv_sec = timeout_ms/1000;
        tv.tv_usec = (timeout_ms%1000)*1000;
        FD_ZERO(&sdset);
        FD_SET(m_socket, &sdset);
        return select(m_socket + 1, &sdset, nullptr, nullptr, &tv) > 0;
    }
    bool resolveHostName(const char* hostname, struct in_addr* addr)
    {
        struct addrinfo *res;
        int result = getaddrinfo(hostname, nullptr, nullptr, &res);
        if (result == 0)
        {
            memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
            freeaddrinfo(res);
        }
        return result != 0;
    }
    void setBlocking()
    {
        long arg = fcntl(m_socket, F_GETFL, nullptr);
        arg |= O_NONBLOCK;
        fcntl(m_socket, F_SETFL, arg);
    }
    void setNonBlocking()
    {
        long arg = fcntl(m_socket, F_GETFL, nullptr);
        arg &= (~O_NONBLOCK);
        fcntl(m_socket, F_SETFL, arg);
    }
public:
    enum  TCPStatus {
        connectionClosed = 0,
        connectionReset = -1,
        connectionTimedOut = -2
    };

    explicit TCPSocket(const int socket): m_socket(socket)
    {
    }
    ~TCPSocket()
    {
        close(m_socket);
    }

    const int raw() const
    {
        return m_socket;
    }

    ssize_t send(const char* buffer, size_t length)
    {
        return write(m_socket, buffer, length);
    }
    ssize_t send(const std::string& message)
    {
        return write(m_socket, message.c_str(), message.size());
    }
    ssize_t receive(char* buffer, size_t length, unsigned int timeout_ms = 0)
    {
        return timeout_ms == 0
            ? read(m_socket, buffer, length)
            : (waitForReadEvent(timeout_ms)
                ? read(m_socket, buffer, length)
                : TCPStatus::connectionTimedOut);
    }

    bool connect(const char* hostname, const int port)
    {
        struct sockaddr_in address;
        socklen_t length = sizeof(address);
        memset(&address, 0, length);
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (!resolveHostName(hostname, &(address.sin_addr)))
        {
            inet_pton(PF_INET, hostname, &(address.sin_addr));
        }
        return ::connect(m_socket, (struct sockaddr*)&address, length) == 0;
    }
    bool connect(const char* hostname, const int port, const unsigned int timeout_ms)
    {
        struct sockaddr_in address;
        socklen_t length = sizeof(address);
        memset(&address, 0, length);
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (!resolveHostName(hostname, &(address.sin_addr)))
        {
            inet_pton(PF_INET, hostname, &(address.sin_addr));
        }

        // Set socket to non-blocking
        setNonBlocking();

        // Connect with time limit
        int result = -1;
        if ((result = ::connect(m_socket, (struct sockaddr *)&address, length)) < 0)
        {
            if (errno == EINPROGRESS)
            {
                struct timeval tv;
                tv.tv_sec = timeout_ms/1000;
                tv.tv_usec = (timeout_ms%1000)*1000;

                fd_set sdset;
                FD_ZERO(&sdset);
                FD_SET(m_socket, &sdset);
                int s = -1;
                do
                {
                    s = select(m_socket + 1, nullptr, &sdset, nullptr, &tv);
                }
                while (s == -1 && errno == EINTR);

                if (s > 0)
                {
                    socklen_t len = sizeof(int);
                    int valopt;
                    getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &len);
                    if (valopt)
                    {
                        return false;
                    }
                    else
                    {
                        // connection established
                        result = 0;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        // Return socket to blocking mode
        setBlocking();

        return result != -1;
    }

    bool bind(const std::string& hostname, const int port)
    {
        struct sockaddr_in address;
        socklen_t length = sizeof(address);
        memset(&address, 0, length);
        address.sin_family = PF_INET;
        address.sin_port = htons(port);
        if (hostname.size() > 0)
            inet_pton(PF_INET, hostname.c_str(), &(address.sin_addr));
        else
            address.sin_addr.s_addr = INADDR_ANY;

        int optval = 1;
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

        return ::bind(m_socket, (struct sockaddr*)&address, length) == 0;
    }
    bool listen()
    {

        return ::listen(m_socket, 5) == 0;
    }
    TCPSocket* accept()
    {
        struct sockaddr_in address;
        socklen_t length = sizeof(address);
        memset(&address, 0, length);
        int accepted = ::accept(m_socket, (struct sockaddr*)&address, &length);
        if (accepted < 0)
            throw TCPException("not accepted");
        return new TCPSocket(accepted);
    }
};
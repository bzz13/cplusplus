#pragma once

#include <string>

class replica
{
    std::string m_hostname;
    int         m_port;
public:
    replica(const std::string& hostname, const int& port)
        : m_hostname(hostname), m_port(port) { }

    const std::string&  host() const { return m_hostname; }
    int                 port() const { return m_port; }
};
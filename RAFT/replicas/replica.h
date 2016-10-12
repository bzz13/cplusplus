#pragma once

#include <string>
#include <iostream>

class replica
{
    std::string m_hostname;
    int         m_port;
public:
    replica()
        : replica("undef", 0) { }

    replica(const replica& other)
        : replica(other.m_hostname, other.m_port) { }

    replica(const std::string& hostname, const int& port)
        : m_hostname(hostname), m_port(port) { }

    const std::string&  host() const { return m_hostname; }
    int                 port() const { return m_port; }

    void operator=(const replica &other )
    {
        m_hostname = other.m_hostname;
        m_port = other.m_port;
    }

    bool operator==(const replica &other )
    {
        return
            m_hostname == other.m_hostname &&
            m_port == other.m_port;
    }

    friend std::ostream& operator<<(std::ostream& os, const replica& r)
    {
        return (os << r.m_hostname << " " << r.m_port);
    }

    friend std::istream& operator>>(std::istream& is, replica& r)
    {
        return (is >> r.m_hostname >> r.m_port);
    }
};
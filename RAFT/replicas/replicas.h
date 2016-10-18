#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "replica.h"

class replicas
{
    std::string             m_filename;
    std::vector<replica>    m_replicas;

    void read()
    {
        if (!m_replicas.empty())
            m_replicas.clear();
        std::ifstream ifs(m_filename, std::ifstream::in);
        while(!ifs.eof())
        {
            std::string host; int port;
            ifs >> host >> port;
            if(host.size() > 0 && host[0] != '#')
                m_replicas.push_back(replica{ host, port });
        }
    }
public:
    replicas(const std::string& filename)
        : m_filename(filename)
    {
        read();
    }

    int size() const { return m_replicas.size(); }
    std::vector<replica>::iterator begin() { return m_replicas.begin(); }
    std::vector<replica>::iterator end() { return m_replicas.end(); }
};
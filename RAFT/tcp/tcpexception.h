#pragma once

#include <stdexcept>
#include <string>

class TCPException: public std::runtime_error
{
public:
    explicit TCPException(const char* what)
        : runtime_error(what) { }
    explicit TCPException(const std::string& what)
        : runtime_error(what) { }
};
#pragma once

#include <memory>
#include <string>
#include <sstream>
#include "server_proto_operation.h"

template<typename TK, typename TV>
class server_proto_parser
{
    typedef server_proto_operation<TK, TV> spo;
    typedef std::shared_ptr<spo> spt_spo;

public:
    spt_spo parse(const std::string& request)
    {
        std::stringstream requestStream(request);
        std::string method;
        requestStream >> method;

        if (method == "stop")       return spt_spo((spo*)new server_proto_stop<TK, TV>());
        if (method == "vote")       return spt_spo((spo*)new server_proto_vote<TK, TV>(requestStream));
        if (method == "hb")         return spt_spo((spo*)new server_proto_heartbeat<TK, TV>(requestStream));
        if (method == "get")        return spt_spo((spo*)new server_proto_get<TK, TV>(requestStream));
        if (method == "del")        return spt_spo((spo*)new server_proto_del<TK, TV>(requestStream));
        if (method == "set")        return spt_spo((spo*)new server_proto_set<TK, TV>(requestStream));
        if (method == "syncset")    return spt_spo((spo*)new server_proto_syncset<TK, TV>(requestStream));
                                    return spt_spo((spo*)new server_proto_undef<TK, TV>());
    }
};
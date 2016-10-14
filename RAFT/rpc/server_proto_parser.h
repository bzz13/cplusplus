#pragma once

#include <memory>
#include <string>
#include <sstream>
#include "server_proto_operation.h"

template<typename TK, typename TV>
class server_proto_parser
{
    typedef std::unique_ptr<server_proto_operation<TK, TV>> upt_spo;

public:
    upt_spo parse(const std::string& request)
    {
        std::stringstream requestStream(request);
        std::string method;
        requestStream >> method;

        if (method == "stop")       return upt_spo(new server_proto_stop<TK, TV>());
        if (method == "vote")       return upt_spo(new server_proto_vote<TK, TV>(requestStream));
        if (method == "hb")         return upt_spo(new server_proto_heartbeat<TK, TV>(requestStream));
        if (method == "get")        return upt_spo(new server_proto_get<TK, TV>(requestStream));
        if (method == "del")        return upt_spo(new server_proto_del<TK, TV>(requestStream));
        if (method == "set")        return upt_spo(new server_proto_set<TK, TV>(requestStream));
        if (method == "syncset")    return upt_spo(new server_proto_syncset<TK, TV>(requestStream));
                                    return upt_spo(new server_proto_undef<TK, TV>());
    }
};
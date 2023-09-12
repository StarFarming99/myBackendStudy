#pragma once

#include <socket/socket.h>

namespace yazi {
namespace socket {

class ServerSocket : public Socket
{
public:
    ServerSocket();
    ServerSocket(const string &ip, int port);
    virtual ~ServerSocket();
};

}}

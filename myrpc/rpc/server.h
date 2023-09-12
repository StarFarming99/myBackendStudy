#pragma once

#include <string>
using std::string;

#include <frame/server.h>
#include <rpc/function_handler.h>

namespace yazi {
namespace rpc {

class Server : public yazi::frame::Server
{
    friend class Singleton<Server>;
public:
    template <typename F>
    void bind(const string & name, F func)
    {
        m_handler.bind(name, func);
    }

    void call(const string & name, DataStream & in, DataStream & out)
    {
        m_handler.call(name, in, out);
    }

private:
    Server();
    ~Server();

private:
    FunctionHandler m_handler;
};

}}

#pragma once

#include <list>
#include <string>
using std::string;

#include <socket/socket.h>
#include <socket/event_poller.h>

#include <utility/object_pool.h>
using namespace yazi::utility;

#include <thread/mutex.h>
#include <thread/task.h>
using namespace yazi::thread;

namespace yazi {
namespace socket {

class SocketHandler
{
public:
    SocketHandler();
    ~SocketHandler();

    void listen(const string & ip, int port);
    void attach(Socket * socket);
    void detach(Socket * socket);
    void remove(Socket * socket);
    void handle(int max_connections, int wait_time);

private:
    EventPoller * m_epoll;
    Socket * m_server;
    ObjectPool<Socket> m_sockpool;
    Mutex m_mutex;
};

}}

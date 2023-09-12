#include <socket/client_socket.h>
using namespace yazi::socket;

#include <utility/logger.h>
using namespace yazi::utility;

#include <cerrno>
#include <cstring>

ClientSocket::ClientSocket() : Socket()
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        log_error("create client socket error: errno=%d errstr=%s", errno, strerror(errno));
    }
}

ClientSocket::ClientSocket(const string& ip, int port) : Socket(ip, port)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        log_error("create client socket error: errno=%d errstr=%s", errno, strerror(errno));
        return;
    }
    Socket::connect(ip, port);
}

ClientSocket::~ClientSocket()
{
    close();
}

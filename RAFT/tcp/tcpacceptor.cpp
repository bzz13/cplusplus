#include <sys/socket.h>

#include "tcpacceptor.h"
#include "tcpexception.h"

TCPAcceptor::TCPAcceptor(int port, const std::string& address):
    m_listning_socket(socket(PF_INET, SOCK_STREAM, 0)),
    m_port(port),
    m_address(address)
{
}

TCPAcceptor::TCPAcceptor(int port, const char* address)
    : TCPAcceptor(port, std::string(address))
{
}

bool TCPAcceptor::start()
{
    if (m_listning == true)
        return false;
    if (!m_listning_socket.bind(m_port, m_address))
        throw TCPException("bind() failed");
    if (!m_listning_socket.listen())
        throw TCPException("listen() failed");
    m_listning = true;
    return m_listning;
}

std::unique_ptr<TCPStream> TCPAcceptor::accept()
{
    if (m_listning == false)
        throw TCPException("accept() failed - not listning");
    return std::unique_ptr<TCPStream>(new TCPStream(
            std::unique_ptr<TCPSocket>(
                m_listning_socket.accept())));
}

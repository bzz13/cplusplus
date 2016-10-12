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

TCPAcceptor::TCPAcceptor(const replica& self)
    : TCPAcceptor(self.port(), self.host())
{
}

bool TCPAcceptor::start()
{
    if (m_listning == true)
        return false;
    if (!m_listning_socket.bind(m_address, m_port))
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
    return std::unique_ptr<TCPStream>(
        new TCPStream(m_listning_socket.accept()));
}

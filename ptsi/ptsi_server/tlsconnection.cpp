#include "tlsconnection.h"
#include "connection_impl.h"

TLSConnection::~TLSConnection()
{
    server_.debugStream_ << "~TLSConnection()" << this << std::endl;
}

void TLSConnection::start()
{
    server_.debugStream_ << "Attempting handshake" << std::endl;
    socket_.async_handshake(boost::asio::ssl::stream_base::server,
                            boost::bind(&TLSConnection::handle_handshake, this->shared_from_this(),
                                        boost::asio::placeholders::error));
}

TLSConnection::TLSConnection(boost::asio::io_service &io_service, PTSIServer &server)
    : BaseConnection<ssl_socket>(io_service, server, server.context_)
{
    server_.debugStream_ << "TLSConnection()" << this << std::endl;
}


TLSConnection::pointer TLSConnection::create(boost::asio::io_service &io_service, PTSIServer &server)
{
    return pointer(new TLSConnection(io_service, server));
}

void TLSConnection::handle_handshake(const boost::system::error_code &error)
{
    if (!error)
    {
        server_.debugStream_ << "Handshake completed" << std::endl;
        BaseConnection<ssl_socket>::start();
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

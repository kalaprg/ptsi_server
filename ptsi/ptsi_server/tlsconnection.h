#ifndef TLSCONNECTION_H
#define TLSCONNECTION_H
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "connection.h"

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
class TLSConnection : public BaseConnection<ssl_socket>
{
public:
    typedef boost::shared_ptr<TLSConnection> pointer;
    ~TLSConnection();
    void start();
    static pointer create(boost::asio::io_service& io_service,
                          PTSIServer &server);

    boost::shared_ptr<TLSConnection> shared_from_this()
    {
        return boost::static_pointer_cast<TLSConnection>(BaseConnection<ssl_socket>::shared_from_this());
    }

private:
    void handle_handshake(const boost::system::error_code& error);
    TLSConnection(boost::asio::io_service& io_service, PTSIServer &server);
};

#endif // TLSCONNECTION_H

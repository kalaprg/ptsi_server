#ifndef INTERNALCONNECTION_H
#define INTERNALCONNECTION_H
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include "common.h"

class PTSIServer;

class InternalConnection
        : public boost::enable_shared_from_this<InternalConnection>
{
public:
    typedef boost::shared_ptr<InternalConnection> pointer;

    static pointer create(boost::asio::io_service& io_service,
                          PTSIServer &server);

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start();

private:
    void handle_command(const boost::system::error_code &error);
    void handle_command2(boost::uint32_t size, const boost::system::error_code &error);
    void handle_nextCommand(const boost::system::error_code &error);

    InternalConnection(boost::asio::io_service& io_service, PTSIServer &server);
    boost::asio::ip::tcp::socket socket_;
    PTSIServer &server_;
    std::vector<char> buffer_;
};

#endif // INTERNALCONNECTION_H

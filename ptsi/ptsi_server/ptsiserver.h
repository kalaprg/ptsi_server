#ifndef PTSISERVER_H
#define PTSISERVER_H
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <vector>
#include <map>
#include "connection.h"
#include "tlsconnection.h"
#include "internalconnection.h"
#include "session.h"
#include "biosignaldata.h"

class PTSIServer
{
public:
    PTSIServer(boost::asio::io_service &io_service,
               std::ostream &errorStream, std::ostream &debugStream);

    boost::shared_ptr<sql::Connection> databaseConnection() const;

private:
    void start_accept();
    void start_acceptInternal();
    void start_acceptSecure();

    void handle_accept(Connection::pointer new_connection,
                       const boost::system::error_code& error);

    void handle_acceptInternal(InternalConnection::pointer new_connection,
                       const boost::system::error_code& error);

    void handle_acceptSecure(TLSConnection::pointer new_connection,
                       const boost::system::error_code& error);

    bool authenticate(const std::string &login, const std::string &password);

    Session::pointer acquireSession(const std::string &devName, boost::uint64_t pesel);

    BiosignalData::pointer getData(boost::uint64_t pesel);

    bool initMySQL();
    void setupSessions();

    std::string getPassword() const;

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::acceptor internalAcceptor_;
    boost::asio::ip::tcp::acceptor secureAcceptor_;
    std::ostream &debugStream_;
    std::ostream &errorStream_;

    boost::shared_ptr<sql::Connection> connection_;

    std::map<boost::uint64_t, Session::pointer> peselSessionMap_;
    std::map<std::string, Session::pointer> deviceSessionMap_;

    boost::asio::ssl::context context_;

    friend class Session;
    friend class BiosignalData;
    template <typename T> friend class BaseConnection;
    friend class TLSConnection;
    friend class InternalConnection;
};

#endif // PTSISERVER_H

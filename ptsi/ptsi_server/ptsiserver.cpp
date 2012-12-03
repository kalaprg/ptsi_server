#include <cstdio>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "common.h"
#include "ptsiserver.h"
#include "connection_impl.h"

PTSIServer::PTSIServer(boost::asio::io_service &io_service,
                       std::ostream &errorStream, std::ostream &debugStream)
    : acceptor_(io_service), internalAcceptor_(io_service, boost::asio::ip::tcp::endpoint(
                                                   boost::asio::ip::tcp::v4(), globalOptions.getInternalPort())),
      secureAcceptor_(io_service),
      debugStream_(debugStream), errorStream_(errorStream),
      context_(boost::asio::ssl::context::tlsv1_server)
{
    if(globalOptions.isUnsecuredConnectionsEnabled())
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), globalOptions.getPort());
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
    }

    if(globalOptions.isSecuredConnectionsEnabled())
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), globalOptions.getSecurePort());
        secureAcceptor_.open(endpoint.protocol());
        secureAcceptor_.set_option(boost::asio::socket_base::reuse_address(true));
        secureAcceptor_.bind(endpoint);
        secureAcceptor_.listen();

        try
        {
            context_.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2);
            context_.use_certificate_file(globalOptions.getCertificateFile().c_str(), boost::asio::ssl::context::pem);
            context_.use_rsa_private_key_file(globalOptions.getPKeyFile().c_str(), boost::asio::ssl::context::pem);
            if(globalOptions.getPKeyPassphrase().size())
            {
                context_.set_password_callback(boost::bind(&PTSIServer::getPassword, this));
            }
        }
        catch(boost::system::system_error &e)
        {
            errorStream_ << "Exception: " << e.what() << std::endl;
            io_service.stop();
            return;
        }
    }

    if(!initMySQL())
    {
        errorStream_ << "Error connecting to database, exiting" << std::endl;
        io_service.stop();
        return;
    }
    setupSessions();
    if(globalOptions.isUnsecuredConnectionsEnabled())
        start_accept();

    if(globalOptions.isSecuredConnectionsEnabled())
        start_acceptSecure();

    start_acceptInternal();
}

boost::shared_ptr<sql::Connection> PTSIServer::databaseConnection() const
{
    return connection_;
}

void PTSIServer::start_accept()
{
    Connection::pointer new_connection =
            Connection::create(acceptor_.get_io_service(), *this);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&PTSIServer::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void PTSIServer::start_acceptInternal()
{
    InternalConnection::pointer internalConnection =
            InternalConnection::create(internalAcceptor_.get_io_service(), *this);

    internalAcceptor_.async_accept(internalConnection->socket(),
                                   boost::bind(&PTSIServer::handle_acceptInternal, this, internalConnection,
                                               boost::asio::placeholders::error));
}

void PTSIServer::start_acceptSecure()
{
    TLSConnection::pointer new_connection =
            TLSConnection::create(acceptor_.get_io_service(), *this);

    secureAcceptor_.async_accept(new_connection->socket().lowest_layer(),
                           boost::bind(&PTSIServer::handle_acceptSecure, this, new_connection,
                                       boost::asio::placeholders::error));
}

void PTSIServer::handle_accept(Connection::pointer new_connection,
                               const boost::system::error_code &error)
{
    if (!error)
    {
        new_connection->start();
    }
    else
    {
        errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }

    start_accept();
}

void PTSIServer::handle_acceptInternal(InternalConnection::pointer new_connection, const boost::system::error_code &error)
{
    if (!error)
    {
        new_connection->start();
    }
    else
    {
        errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }

    start_acceptInternal();
}

void PTSIServer::handle_acceptSecure(TLSConnection::pointer new_connection, const boost::system::error_code &error)
{
    if (!error)
    {
        new_connection->start();
    }
    else
    {
        errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }

    start_acceptSecure();
}

bool PTSIServer::authenticate(const std::string &login, const std::string &password)
{
    try
    {
        boost::scoped_ptr<sql::Statement> stmt(connection_->createStatement());
        std::string query = "SELECT privileges "
                "FROM users WHERE login='" + login + "' AND password='" + password + "'";
        boost::scoped_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        if(res->next())
        {
            return res->getInt("privileges") > 0;
        }
    }
    catch(sql::SQLException &e)
    {
        errorStream_ << e.what() << std::endl;
    }
    return false;
}

Session::pointer PTSIServer::acquireSession(const std::string &devName, boost::uint64_t pesel)
{
    std::map<std::string, Session::pointer>::iterator it =
            deviceSessionMap_.find(devName);

    if(it != deviceSessionMap_.end())
    {
        if(pesel)
        {
            if(it->second->isAssociated() && it->second->getPesel() == pesel)
            {
                return it->second;
            }
            else
            {
                std::map<boost::uint64_t, Session::pointer>::iterator it2 =
                        peselSessionMap_.find(it->second->getPesel());

                Session::pointer session(it2->second);
                peselSessionMap_.erase(it2);
                if(session->associateWithPatient(pesel, session->getDeviceName()))
                {
                    peselSessionMap_.insert(
                                std::pair<boost::uint64_t,Session::pointer>(session->getPesel(), session));
                    return session;
                }
                else
                {
                    deviceSessionMap_.erase(it);
                    return Session::pointer();
                }
            }
        }
        else
        {
            if(it->second->isAssociated())
            {
                return it->second;
            }
            else
            {
                return Session::pointer();
            }
        }
    }
    else
    {
        Session::pointer session(Session::create(*this));
        if(pesel && session->associateWithPatient(pesel, devName))
        {
            peselSessionMap_.insert(
                        std::pair<boost::uint64_t,Session::pointer>(session->getPesel(), session));
            deviceSessionMap_.insert(
                        std::pair<std::string, Session::pointer>(session->getDeviceName(), session));
            return session;
        }
        else
            return Session::pointer();
    }
}

BiosignalData::pointer PTSIServer::getData(boost::uint64_t pesel)
{
    std::map<boost::uint64_t, Session::pointer>::iterator it =
            peselSessionMap_.find(pesel);

    if(it == peselSessionMap_.end())
        return BiosignalData::pointer();

    return it->second->getData();
}

bool PTSIServer::initMySQL()
{
    try
    {
        connection_.reset();
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::ConnectOptionsMap options;
        options["hostName"] = sql::SQLString(globalOptions.getMySQLServer());
        options["userName"] = sql::SQLString(globalOptions.getMySQLUser());
        options["password"] = sql::SQLString(globalOptions.getMySQLPassword());
        options["CLIENT_MULTI_STATEMENTS"] = true;
        boost::shared_ptr<sql::Connection> connection(driver->connect(options));

        boost::scoped_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->execute("USE " + globalOptions.getMySQLDatabase());

        connection_ = connection;
        return true;
    }
    catch(sql::SQLException &e)
    {
        errorStream_ << e.what() << std::endl;
    }
    return false;
}

void PTSIServer::setupSessions()
{
    std::list<Session::pointer> sessions;
    Session::getUnexpiredSessions(*this, sessions);
    for(std::list<Session::pointer>::iterator it = sessions.begin();
        it != sessions.end(); ++it)
    {
        peselSessionMap_.insert(std::pair<boost::uint64_t, Session::pointer>((*it)->getPesel(), *it));
        deviceSessionMap_.insert(std::pair<std::string, Session::pointer>((*it)->getDeviceName(), *it));
    }
}

std::string PTSIServer::getPassword() const
{
    return globalOptions.getPKeyPassphrase();
}

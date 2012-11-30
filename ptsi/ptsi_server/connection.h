#ifndef CONNECTION_H
#define CONNECTION_H
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include "session.h"
#include "common.h"

class PTSIServer;

class Connection
        : public boost::enable_shared_from_this<Connection>
{
public:
    typedef boost::shared_ptr<Connection> pointer;

    static pointer create(boost::asio::io_service& io_service,
                          PTSIServer &server);

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start();

    ~Connection();
private:
    Connection(boost::asio::io_service& io_service, PTSIServer &server);

    void read_header(const boost::system::error_code &error);

    void do_read_string(std::string &str, boost::function<void()> &callback);

    void read_string(std::string &str, boost::function<void()> &callback,
                     const boost::system::error_code &error);

    void read_string2(std::string &str, boost::function<void()> &callback,
                      const boost::system::error_code &error);

    void do_read_uint64(boost::uint64_t &data, boost::function<void()> &callback);

    void read_uint64(boost::uint64_t &data, boost::function<void()> &callback,
                     const boost::system::error_code &error);

    void handle_serverResponse(const boost::system::error_code &error);

    void do_setPatient(SetPatientPacket::pointer patient);

    void readTransmissionSetup(const boost::system::error_code &error);

    void readBioSignals(const boost::system::error_code &error);

    void readBioSignals2(boost::uint32_t size, const boost::system::error_code &error);

    boost::asio::ip::tcp::socket socket_;
    PTSIServer &server_;
    std::vector<char> buffer_;
    std::string hostname_;
    Session::pointer session_;
};

#endif // CONNECTION_H

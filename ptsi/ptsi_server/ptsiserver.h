#ifndef PTSISERVER_H
#define PTSISERVER_H
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <vector>

class PTSIServer
{
public:
    PTSIServer(boost::asio::io_service &io_service, unsigned int port,
               std::ostream &errorStream, std::ostream &debugStream);

private:

    enum PacketType {SET_PATIENT = 0, SETUP_TRANSMISSION, BIOSIGNALS};

    class Connection;

    class SetPatientPacket
            : public boost::enable_shared_from_this<SetPatientPacket>
    {
    public:
        typedef boost::shared_ptr<SetPatientPacket> pointer;
        SetPatientPacket();
        const std::string &login() const;
        const std::string &password() const;
        boost::uint64_t pesel() const;
        friend class PTSIServer::Connection;
    private:
        std::string login_;
        std::string password_;
        boost::uint64_t pesel_;
    };

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

        boost::asio::ip::tcp::socket socket_;
        PTSIServer &server_;
        std::vector<char> buffer_;
        std::string hostname_;
    };

    void start_accept();

    void handle_accept(Connection::pointer new_connection,
                       const boost::system::error_code& error);

    bool authenticate(const std::string &login, const std::string &password);

    boost::asio::ip::tcp::acceptor acceptor_;
    std::ostream &debugStream_;
    std::ostream &errorStream_;
};

#endif // PTSISERVER_H

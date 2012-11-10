#include <boost/asio.hpp>
#include <boost/function.hpp>
#include "ptsiserver.h"

PTSIServer::PTSIServer(boost::asio::io_service &io_service, unsigned int port,
                       std::ostream &errorStream, std::ostream &debugStream)
    : acceptor_(io_service, boost::asio::ip::tcp::endpoint(
                    boost::asio::ip::tcp::v4(), port)),
      debugStream_(debugStream), errorStream_(errorStream)
{
    start_accept();
}

void PTSIServer::start_accept()
{
    Connection::pointer new_connection =
            Connection::create(acceptor_.get_io_service(), *this);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&PTSIServer::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void PTSIServer::handle_accept(PTSIServer::Connection::pointer new_connection,
                               const boost::system::error_code &error)
{
    if (!error)
    {
        new_connection->start();
    }

    start_accept();
}

bool PTSIServer::authenticate(const std::string &login, const std::string &password)
{
    //TODO: connect to db and really authenticate!
    return (login.compare("test") == 0) && (password.compare("pass123") == 0);
}

PTSIServer::Connection::pointer
PTSIServer::Connection::create(boost::asio::io_service &io_service,
                               PTSIServer &server)
{
    return pointer(new Connection(io_service, server));
}

void PTSIServer::Connection::start()
{
    boost::asio::ip::tcp::endpoint endpoint = socket_.remote_endpoint();
    boost::asio::ip::tcp::resolver resolver(socket_.get_io_service());
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(endpoint);
    hostname_ = it->host_name();
    buffer_.resize(1);
    boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
                            boost::bind(&Connection::read_header,
                                        shared_from_this(),
                                        boost::asio::placeholders::error));
}

void PTSIServer::Connection::read_header(const boost::system::error_code &error)
{
    if(!error)
    {
        switch((PacketType) buffer_[0])
        {
        case SET_PATIENT:
        {
            PTSIServer::SetPatientPacket::pointer packet(
                        new PTSIServer::SetPatientPacket());

            boost::function<void()> finalCallback(
                        boost::bind(&Connection::do_setPatient, shared_from_this(),
                                    packet));

            boost::function<void()> peselCallback(
                        boost::bind(&Connection::do_read_uint64, shared_from_this(),
                                    boost::ref(packet->pesel_), finalCallback));

            boost::function<void()> passwordCallback(
                        boost::bind(&Connection::do_read_string, shared_from_this(),
                                    boost::ref(packet->password_), peselCallback));

            do_read_string(packet->login_, passwordCallback);

            break;
        }
        case SETUP_TRANSMISSION:
            break;
        case BIOSIGNALS:
            break;
        default:
            server_.debugStream_ << "Unrecognized packet type: "
                                 << std::hex << (int) buffer_[0] << std::endl;
        }
    }
}

void PTSIServer::Connection::do_read_string(std::string &str, boost::function<void()> &callback)
{
    if(!buffer_.size())
        buffer_.resize(1);

    boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 1),
                            boost::bind(&Connection::read_string,
                                        shared_from_this(),
                                        boost::ref(str), callback,
                                        boost::asio::placeholders::error));
}

void PTSIServer::Connection::read_string(std::string &str,
                                         boost::function<void()> &callback,
                                         const boost::system::error_code &error)
{
    if(!error)
    {
        buffer_.resize(buffer_[0]);
        boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
                                boost::bind(&Connection::read_string2,
                                            shared_from_this(),
                                            boost::ref(str), callback,
                                            boost::asio::placeholders::error));
    }
}

void PTSIServer::Connection::read_string2(std::string &str,
                                          boost::function<void()> &callback,
                                          const boost::system::error_code &error)
{
    if(!error)
    {
        str.assign(buffer_.begin(), buffer_.end());
        callback();
    }
}

void PTSIServer::Connection::do_read_uint64(boost::uint64_t &data,
                                            boost::function<void()> &callback)
{
    buffer_.resize(sizeof(data));
    boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
                            boost::bind(&Connection::read_uint64,
                                        shared_from_this(),
                                        boost::ref(data), callback,
                                        boost::asio::placeholders::error));
}

void PTSIServer::Connection::read_uint64(boost::uint64_t &data,
                                         boost::function<void()> &callback,
                                         const boost::system::error_code &error)
{
    if(!error)
    {
        void *ptr = &buffer_.front();
        data = *static_cast<boost::uint64_t*>(ptr);
        callback();
    }
}

void PTSIServer::Connection::handle_serverResponse(const boost::system::error_code &error)
{
    if(!error)
    {

    }
}

void PTSIServer::Connection::do_setPatient(SetPatientPacket::pointer patient)
{
    server_.debugStream_ << "Login: " << patient->login() << " password: "
                            << patient->password() << std::endl
                            <<  "pesel " << patient->pesel() << std::endl;

    if(buffer_.size() < 2)
        buffer_.resize(2);

    buffer_[0] = 0;

    if(server_.authenticate(patient->login(), patient->password()))
    {
        buffer_[1] = 1;
    }
    else
    {
        buffer_[1] = 0;
    }
    boost::asio::async_write(socket_, boost::asio::buffer(buffer_, 2),
                             boost::bind(&Connection::handle_serverResponse,
                                         shared_from_this(),
                                         boost::asio::placeholders::error()));
}

PTSIServer::Connection::Connection(boost::asio::io_service &io_service,
                                   PTSIServer &server)
    : socket_(io_service), server_(server)
{
}

PTSIServer::SetPatientPacket::SetPatientPacket()
    : pesel_(0)
{
}

const std::string &PTSIServer::SetPatientPacket::login() const
{
    return login_;
}

const std::string &PTSIServer::SetPatientPacket::password() const
{
    return password_;
}

boost::uint64_t PTSIServer::SetPatientPacket::pesel() const
{
    return pesel_;
}

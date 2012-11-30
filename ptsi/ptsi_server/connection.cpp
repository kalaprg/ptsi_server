#include <boost/asio.hpp>
#include "ptsiserver.h"
#include "connection.h"

Connection::pointer
Connection::create(boost::asio::io_service &io_service,
                   PTSIServer &server)
{
    return pointer(new Connection(io_service, server));
}

void Connection::start()
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

void Connection::read_header(const boost::system::error_code &error)
{
    if(!error)
    {
        switch((PacketType) buffer_[0])
        {
        case SET_PATIENT:
        {
            SetPatientPacket::pointer packet(
                        new SetPatientPacket());

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
            server_.debugStream_ << "Received setup transmission packet" << std::endl;
            if(buffer_.size() < sizeof(TransmissionSetup))
                buffer_.resize(sizeof(TransmissionSetup));

            boost::asio::async_read(socket_, boost::asio::buffer(buffer_, sizeof(TransmissionSetup)),
                                    boost::bind(&Connection::readTransmissionSetup,
                                                shared_from_this(),
                                                boost::asio::placeholders::error));

            break;
        case BIOSIGNALS:
            server_.debugStream_ << "Received data packet" << std::endl;
            if(buffer_.size() < sizeof(boost::uint16_t))
                buffer_.resize(sizeof(boost::uint16_t));

            boost::asio::async_read(socket_, boost::asio::buffer(buffer_, sizeof(boost::uint16_t)),
                                    boost::bind(&Connection::readBioSignals,
                                                shared_from_this(),
                                                boost::asio::placeholders::error));

            break;
        default:
            server_.debugStream_ << "Unrecognized packet type: "
                                 << std::hex << (int) buffer_[0] << std::endl;
        }
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void Connection::do_read_string(std::string &str, boost::function<void()> &callback)
{
    if(!buffer_.size())
        buffer_.resize(1);

    boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 1),
                            boost::bind(&Connection::read_string,
                                        shared_from_this(),
                                        boost::ref(str), callback,
                                        boost::asio::placeholders::error));
}

void Connection::read_string(std::string &str,
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
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void Connection::read_string2(std::string &str,
                              boost::function<void()> &callback,
                              const boost::system::error_code &error)
{
    if(!error)
    {
        str.assign(buffer_.begin(), buffer_.end());
        callback();
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void Connection::do_read_uint64(boost::uint64_t &data,
                                boost::function<void()> &callback)
{
    buffer_.resize(sizeof(data));
    boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
                            boost::bind(&Connection::read_uint64,
                                        shared_from_this(),
                                        boost::ref(data), callback,
                                        boost::asio::placeholders::error));
}

void Connection::read_uint64(boost::uint64_t &data,
                             boost::function<void()> &callback,
                             const boost::system::error_code &error)
{
    if(!error)
    {
        void *ptr = &buffer_.front();
        data = *static_cast<boost::uint64_t*>(ptr);
        callback();
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void Connection::handle_serverResponse(const boost::system::error_code &error)
{
    if(!error)
    {
        //try to read next packet
        if(buffer_.size() == 0)
            buffer_.resize(1);

        boost::asio::async_read(socket_, boost::asio::buffer(buffer_, 1),
                                boost::bind(&Connection::read_header,
                                            shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void Connection::do_setPatient(SetPatientPacket::pointer patient)
{
    server_.debugStream_ << "Login: " << patient->login() << " password: "
                         << patient->password() << std::endl
                         <<  "pesel " << patient->pesel() << std::endl;

    if(buffer_.size() < 2)
        buffer_.resize(2);

    buffer_[0] = 0;

    if(server_.authenticate(patient->login(), patient->password()))
    {
        session_ = server_.acquireSession(hostname_, patient->pesel());
        if(session_ && session_->isAssociated())
        {
            buffer_[1] = 1;
        }
        else
        {
            //FIXME maybe add additional error code?
            buffer_[1] = 0;
        }
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

void Connection::readTransmissionSetup(const boost::system::error_code &error)
{
    char response;
    if(!error)
    {
        if(session_ && session_->isAssociated())
        {
            TransmissionSetup *setup =
                    (TransmissionSetup *)(&buffer_.front());

            server_.debugStream_ << "readTransmissionSetup() "
                                 << setup->samplingFrequency_  << " " << setup->ecgFrameSize_ << std::endl;
            if(setup->samplingFrequency_ > 0 && setup->ecgFrameSize_ > 0)
            {
                if(session_->setupTransmission(*setup))
                    response = 0;
                else
                    response = 1;
            }
            else
            {
                response = 1;
            }
        }
        else
        {
            response = 2;
        }
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
        response = 1;
    }

    if(buffer_.size() < 2)
        buffer_.resize(2);

    buffer_[0] = 1;
    buffer_[1] = response;
    boost::asio::async_write(socket_, boost::asio::buffer(buffer_, 2),
                             boost::bind(&Connection::handle_serverResponse,
                                         shared_from_this(),
                                         boost::asio::placeholders::error()));
}

void Connection::readBioSignals(const boost::system::error_code &error)
{
    if(!error)
    {
        boost::uint16_t size = *((boost::uint16_t *) &buffer_.front());
        if(buffer_.size() < size)
            buffer_.resize(size);

        boost::asio::async_read(socket_, boost::asio::buffer(buffer_, size),
                                 boost::bind(&Connection::readBioSignals2,
                                             shared_from_this(), size,
                                             boost::asio::placeholders::error()));
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void Connection::readBioSignals2(boost::uint32_t size, const boost::system::error_code &error)
{
    if(!session_)
        session_ = server_.acquireSession(hostname_, 0);

    char response;
    DataPacket packet;
    if(!error)
    {
        int idx = 0;
        packet.frameNumber_ = *((boost::uint32_t*) &buffer_[idx]); idx += sizeof(boost::uint32_t);
        packet.saturation_ = buffer_[idx++];
        packet.pulse_ =  buffer_[idx++];
        uint ecgNum = size - idx;
        packet.ecg_.resize(ecgNum);
        for(uint i = 0; i < ecgNum; ++i, ++idx)
        {
            packet.ecg_[i] = buffer_[idx];
        }

        server_.debugStream_ << "Received ecg frame #" << packet.frameNumber_ << " of size " << ecgNum;
//        for(uint i = 0; i < ecgNum; ++i, ++idx)
//        {
//            server_.debugStream_ << " " << (int)(char) packet.ecg_[i];
//        }
        server_.debugStream_ << std::endl;

        if(session_ && session_->isAssociated())
        {
            if(session_->isTransmissionSetup() && (ecgNum % session_->getFrameSize() == 0))
            {
                response = 0;
            }
            else
                response = 1;
        }
        else
        {
            response = 2;
        }
    }
    else
    {
        response = 1;
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }

    if(buffer_.size() < 2)
        buffer_.resize(2);

    buffer_[0] = 2;
    buffer_[1] = response;
    boost::asio::async_write(socket_, boost::asio::buffer(buffer_, 2),
                             boost::bind(&Connection::handle_serverResponse,
                                         shared_from_this(),
                                         boost::asio::placeholders::error()));

    if(response == 0)
    {
        session_->getData()->storePacket(packet);
    }
}

Connection::Connection(boost::asio::io_service &io_service,
                       PTSIServer &server)
    : socket_(io_service), server_(server)
{
    server_.debugStream_ << "Connection()" << std::endl;
}

Connection::~Connection()
{
    server_.debugStream_ << "~Connection()" << std::endl;
}

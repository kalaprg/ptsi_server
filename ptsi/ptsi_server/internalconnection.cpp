#include "ptsiserver.h"
#include "internalconnection.h"
InternalConnection::pointer
InternalConnection::create(boost::asio::io_service &io_service,
                           PTSIServer &server)
{
    return pointer(new InternalConnection(io_service, server));
}

void InternalConnection::start()
{
    buffer_.resize(sizeof(boost::uint32_t));
    boost::asio::async_read(socket_, boost::asio::buffer(buffer_),
                            boost::bind(&InternalConnection::handle_command,
                                        shared_from_this(),
                                        boost::asio::placeholders::error));
}

void InternalConnection::handle_command(const boost::system::error_code &error)
{
    if(!error)
    {
        boost::uint32_t size;
        size = *(boost::uint32_t *)&buffer_.front();

        if(buffer_.size() < size)
            buffer_.resize(size);

        server_.debugStream_ << "Received packet of size " << size << std::endl;

        boost::asio::async_read(socket_, boost::asio::buffer(buffer_, size),
                                boost::bind(&InternalConnection::handle_command2,
                                            shared_from_this(), size,
                                            boost::asio::placeholders::error));
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void InternalConnection::handle_command2(boost::uint32_t size, const boost::system::error_code &error)
{
    if(!error)
    {
        if(size < sizeof(boost::uint64_t) + 2)
            return;

        size_t restSize = size - (sizeof(boost::uint64_t) + 2);

        int idx = 0;
        boost::uint64_t pesel = *(((boost::uint64_t*)&buffer_[idx])); idx += sizeof(boost::uint64_t);
        unsigned char readerId = buffer_[idx++];
        enum Command {GET_SIZE = 0, READ, INIT};
        Command cmd = (Command) buffer_[idx++];
        switch(cmd)
        {
        case GET_SIZE:
        {
            if(restSize != 1)
                return;

            unsigned char dataType = buffer_[idx++];
            BiosignalData:: pointer data(server_.getData(pesel));
            if(!data)
                return;
            size_t size = data->getSize(dataType, readerId);
            if(buffer_.size() < sizeof(boost::uint32_t))
                buffer_.resize(sizeof(boost::uint32_t));

            idx = 0;
            *(((boost::uint32_t*)&buffer_[idx])) = size; idx += sizeof(boost::uint32_t);

            boost::asio::async_write(socket_, boost::asio::buffer(buffer_, sizeof(boost::uint32_t)),
                                     boost::bind(&InternalConnection::handle_nextCommand,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error));

            break;
        }
        case READ:
        {
            if(restSize != 1 + sizeof(boost::uint32_t))
                return;

            unsigned char dataType = buffer_[idx++];
            boost::uint32_t size = *(((boost::uint32_t*)&buffer_[idx])); idx += sizeof(boost::uint32_t);
            BiosignalData:: pointer data(server_.getData(pesel));
            if(!data)
                return;

            std::vector<unsigned char> buf(size);
            data->readBytes(dataType, readerId, buf);
            buffer_.resize(size + sizeof(boost::uint32_t));
            idx = 0;
            *(((boost::uint32_t*)&buffer_[idx])) = size; idx += sizeof(boost::uint32_t);
            memcpy(&buffer_[idx], &buf.front(), size);

            server_.errorStream_ << "READ(" << (int)dataType << "):";
            for(int i = 0; i<(int)size; ++i)
                server_.errorStream_ << " " << (unsigned int)(unsigned char)buffer_[i + idx];
            server_.errorStream_ << std::endl;

            boost::asio::async_write(socket_, boost::asio::buffer(buffer_),
                    boost::bind(&InternalConnection::handle_nextCommand,
                                shared_from_this(),
                                boost::asio::placeholders::error));

            break;
        }
        case INIT:
        {
            if(restSize != 1 + sizeof(boost::uint32_t))
                return;

            unsigned char dataType = buffer_[idx++];
            boost::uint32_t size = *(((boost::uint32_t*)&buffer_[idx])); idx += sizeof(boost::uint32_t);
            BiosignalData:: pointer data(server_.getData(pesel));
            if(!data)
                return;
            data->init(dataType, readerId, size);

            return handle_nextCommand(boost::system::error_code());

            break;
        }
        default:
            server_.errorStream_ << "Unrecognized command:" << (int) cmd << std::endl;
            return;
        }
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

void InternalConnection::handle_nextCommand(const boost::system::error_code &error)
{
    if(!error)
    {
        if(buffer_.size() < sizeof(boost::uint32_t))
            buffer_.resize(sizeof(boost::uint32_t));

        boost::asio::async_read(socket_, boost::asio::buffer(buffer_, sizeof(boost::uint32_t)),
                                boost::bind(&InternalConnection::handle_command,
                                            shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    else
    {
        server_.errorStream_ << __FUNCTION__ << " Error: " << error.message() << std::endl;
    }
}

InternalConnection::InternalConnection(boost::asio::io_service &io_service,
                                       PTSIServer &server)
    : socket_(io_service), server_(server)
{
}

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
        //FIXME: check size?
        boost::uint64_t pesel = *(boost::uint64_t *)&buffer_[0];
        char dataType = buffer_[sizeof(pesel)];
        char *fileNames[3] = {"test0", "test1", "test2"};
        server_.debugStream_ << "Command for " << pesel << " " << (int)dataType;
        int idx = sizeof(pesel) + 1;
        if(buffer_[idx] == 0)
        {
            FILE *file = fopen(fileNames[dataType], "rb");
            fseek(file, 0, SEEK_END);
            boost::uint32_t length = ftell(file);
            fclose(file);

            server_.debugStream_ << " Get length of file " << fileNames[dataType] << ": " << length << std::endl;

            if(buffer_.size() < sizeof(length))
                buffer_.resize(size);

            *((boost::uint32_t*)&buffer_.front()) = length;

            boost::asio::async_write(socket_, boost::asio::buffer(buffer_, sizeof(length)),
                                     boost::bind(&InternalConnection::handle_nextCommand,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error));

        }
        else if(buffer_[idx] == 1)
        {
            ++idx;
            boost::uint32_t offset = *(boost::uint32_t *)&buffer_[idx];
            boost::uint32_t length = *(boost::uint32_t *)&buffer_[idx + sizeof(offset)];

            if(buffer_.size() < length + sizeof(length))
                buffer_.resize(length + sizeof(length));

            server_.debugStream_ << " Read file: " << length << " bytes at offset " << offset << std::endl;

            FILE *file = fopen(fileNames[dataType], "rb");
            fseek(file, offset, SEEK_SET);
            length = fread(&buffer_[sizeof(length)], 1, length, file);

            server_.debugStream_ << length << " bytes was read" << std::endl;

            fclose(file);
            *((boost::uint32_t*)&buffer_.front()) = length;
            int bufsize = length + sizeof(length);

            boost::asio::async_write(socket_, boost::asio::buffer(buffer_, bufsize),
                                     boost::bind(&InternalConnection::handle_nextCommand,
                                                 shared_from_this(),
                                                 boost::asio::placeholders::error));
        }
        else
        {
            server_.debugStream_ << " UNKNOWN COMMAND" << std::endl;
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

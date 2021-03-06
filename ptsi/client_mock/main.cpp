//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/cstdint.hpp>
#include <unistd.h>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        std::cout << "endpoint " << endpoint_iterator->endpoint().address();
        std::cout << ":" << endpoint_iterator->endpoint().port() << std::endl;

        std::string login("test");
        std::string password("pass123");

        const size_t size = 3 + login.size() + password.size() + sizeof(boost::uint64_t);
        std::vector<char> buf(size);
        buf[0] = 0;
        buf[1] = login.size();
        int i = 2;
        for(std::string::const_iterator it = login.begin(); it != login.end();
            ++it)
        {
            buf[i++] = *it;
        }
        buf[i++] = password.size();
        for(std::string::const_iterator it = password.begin(); it != password.end();
            ++it)
        {
            buf[i++] = *it;
        }

        void *ptr = &buf[i];
        *(static_cast<boost::uint64_t *>(ptr)) = 44051401458;

        tcp::socket socket(io_service);
        std::cerr << "Connecting... ";
        boost::asio::connect(socket, endpoint_iterator);
        std::cerr << std::endl << "Logging in: " << std::endl;
        boost::asio::write(socket, boost::asio::buffer(buf));
        buf.resize(2);
        boost::asio::read(socket, boost::asio::buffer(buf));
        if(buf[0] == 0)
        {
            if(buf[1] == 1)
                std::cerr << "SUCCESS" << std::endl;
            else
            {
                std::cerr << "FAILED" << std::endl;
                return EXIT_SUCCESS;
            }
        }
        else
        {
            std::cerr << "Incorrect server response: " << std::hex << (int)buf[0]
                      << std::hex << (int)buf[1] << std::endl;

            return EXIT_SUCCESS;
        }
        buf.resize(5);
        buf[0] = 1;
        ptr = &buf[1];
        *(static_cast<boost::uint16_t *>(ptr)) = 20;
        ptr = &buf[3];
        *(static_cast<boost::uint16_t *>(ptr)) = 40;

        std::cerr << std::endl << "Configuring transmission: " << std::endl;
        boost::asio::write(socket, boost::asio::buffer(buf));
        buf.resize(2);
        boost::asio::read(socket, boost::asio::buffer(buf));
        if(buf[0] == 1)
        {
            if(buf[1] == 0)
                std::cerr << "SUCCESS" << std::endl;
            else if(buf[1] == 1)
            {
                std::cerr << "BAD DATA" << std::endl;
                return EXIT_SUCCESS;
            }
            else
            {
                std::cerr << "NO AUTHENTICATION" << std::endl;
                return EXIT_SUCCESS;
            }
        }
        else
        {
            std::cerr << "Incorrect server response: " << std::hex << (int)buf[0]
                      << std::hex << (int)buf[1] << std::endl;

            return EXIT_SUCCESS;
        }

        const int frameSize = 1 + 2 + 4 + 2 + 40;
        const int dataLength = 4 + 2 + 40;
        buf.resize(frameSize);
        boost::uint32_t frameNum = 0;

        int cntr1, cntr2;
        while(true)
        {
            cntr1 = frameNum % 16;
            cntr2 = frameNum % 32;
            int idx = 0;
            buf[idx++] = 2;
            *((boost::uint16_t*)&buf[idx]) = dataLength; idx += 2;
            *((boost::uint32_t*)&buf[idx]) = frameNum; idx += 4;
            buf[idx++] = cntr1;
            buf[idx++] = cntr2;
            for(int i = 0; i < 40; ++i, ++idx)
            {
                buf[idx] = (1.0 + sin(2*M_PI * i / 40.0)) * 127.0;
            }
            sleep(2);//for monitor
//            usleep(500000);
            boost::asio::write(socket, boost::asio::buffer(buf));
            boost::asio::read(socket, boost::asio::buffer(buf,2));
            frameNum++;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

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
#include <queue>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/cstdint.hpp>
#include <unistd.h>
#include <ncurses.h>

using boost::asio::ip::tcp;

void sendInit(tcp::socket &socket, int dataType, int size)
{
    std::vector<unsigned char> buffer(4+8+1+1+1+4);
    int idx = 0;
    *((boost::uint32_t*)&buffer[idx]) = 8+1+1+1+4; idx += sizeof(boost::uint32_t);
    *((boost::uint64_t*)&buffer[idx]) = 44051401458; idx += sizeof(boost::uint64_t);
    buffer[idx++] = 16;//readerID
    buffer[idx++] = 2;
    buffer[idx++] = dataType;
    *((boost::uint32_t*)&buffer[idx]) = size; idx += sizeof(boost::uint32_t);
    boost::asio::write(socket, boost::asio::buffer(buffer));
}

int getSize(tcp::socket &socket, int dataType)
{
    std::vector<unsigned char> buffer(4+8+1+1+1);
    int idx = 0;
    *((boost::uint32_t*)&buffer[idx]) = 8+1+1+1; idx += sizeof(boost::uint32_t);
    *((boost::uint64_t*)&buffer[idx]) = 44051401458; idx += sizeof(boost::uint64_t);
    buffer[idx++] = 16;//readerID
    buffer[idx++] = 0;
    buffer[idx++] = dataType;
    boost::asio::write(socket, boost::asio::buffer(buffer));
    buffer.resize(4);
    boost::asio::read(socket, boost::asio::buffer(buffer));
    return *((boost::uint32_t*)&buffer[0]);
}

void readData(tcp::socket &socket, int dataType, std::vector<unsigned char> &bytes)
{
    std::vector<unsigned char> buffer(4+8+1+1+1+4);
    int idx = 0;
    *((boost::uint32_t*)&buffer[idx]) = 8+1+1+1+4; idx += sizeof(boost::uint32_t);
    *((boost::uint64_t*)&buffer[idx]) = 44051401458; idx += sizeof(boost::uint64_t);
    buffer[idx++] = 16;//readerID
    buffer[idx++] = 1;
    buffer[idx++] = dataType;
    *((boost::uint32_t*)&buffer[idx]) = bytes.size(); idx += sizeof(boost::uint32_t);
    boost::asio::write(socket, boost::asio::buffer(buffer));
    buffer.resize(4);
    boost::asio::read(socket, boost::asio::buffer(buffer));
    int size = *((boost::uint32_t*)&buffer[0]);
    bytes.resize(size);
    boost::asio::read(socket, boost::asio::buffer(bytes));
}

void updateScreen(unsigned char oxy, unsigned char pulse,
                  std::queue<unsigned char> &data, int size)
{
    erase();
    for(int i = 0; i < 80; ++i)
    {
        unsigned char sample = data.front();

        unsigned char x = 79 - i;
        unsigned char y = (1.0 - sample / 255.0) * 23;

        mvaddch(y, x, '*');

        data.pop();
        if(i >= size)
            data.push(sample);
    }

    for(int i = 0; i < size; ++i)
    {
        unsigned char sample = data.front();
        data.pop();
        data.push(sample);
    }

    mvprintw(23, 0, "SAT: %d PULSE: %d", (int)oxy, (int)pulse);
    refresh();
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: monitor <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        std::cout << "endpoint " << endpoint_iterator->endpoint().address();
        std::cout << ":" << endpoint_iterator->endpoint().port() << std::endl;

        tcp::socket socket(io_service);
        std::cerr << "Connecting... ";
        boost::asio::connect(socket, endpoint_iterator);

//        int frameSize = 40;
//        int samplingFreq = 20;
        initscr();
        cbreak();
        noecho();


        sendInit(socket, 0, 2);
        sendInit(socket, 1, 2);
        sendInit(socket, 2, 80);
        std::vector<unsigned char> ecg(80);
        std::vector<unsigned char> pulse(2);
        std::vector<unsigned char> oxygen(2);
        std::queue<unsigned char> ecgQueue;
        for(int i = 0; i < 80; ++i)
            ecgQueue.push(0);

        while(true)
        {
            //get size for test, but ignore it
            getSize(socket, 0);
            getSize(socket, 1);
            getSize(socket, 2);
            readData(socket, 0, oxygen);
            readData(socket, 1, pulse);
            readData(socket, 2, ecg);
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[0 + i]);
            updateScreen(oxygen[0], pulse[0], ecgQueue, 8);
            usleep(400000);//t = 0.4
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[8 + i]);
            updateScreen(oxygen[0], pulse[0], ecgQueue, 8);
            usleep(400000);//t = 0.8
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[16 + i]);
            updateScreen(oxygen[0], pulse[0], ecgQueue, 8);
            usleep(400000);//t = 1.2
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[24 + i]);
            updateScreen(oxygen[0], pulse[0], ecgQueue, 8);
            usleep(400000);//t = 1.6
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[32 + i]);
            updateScreen(oxygen[0], pulse[0], ecgQueue, 8);
            usleep(400000);//t = 2.0
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[40 + i]);
            updateScreen(oxygen[1], pulse[1], ecgQueue, 8);
            usleep(400000);//t = 2.4
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[48 + i]);
            updateScreen(oxygen[1], pulse[1], ecgQueue, 8);
            usleep(400000);//t = 2.8
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[56 + i]);
            updateScreen(oxygen[1], pulse[1], ecgQueue, 8);
            usleep(400000);//t = 3.2
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[64 + i]);
            updateScreen(oxygen[1], pulse[1], ecgQueue, 8);
            usleep(400000);//t = 3.6
            for(int i = 0; i < 8; ++i) ecgQueue.push(ecg[72 + i]);
            updateScreen(oxygen[1], pulse[1], ecgQueue, 8);
            usleep(400000);//t = 4.0
        }

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


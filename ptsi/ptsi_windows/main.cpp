#include <iostream>
#include <boost/asio.hpp>
#include "ptsiserver.h"

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        PTSIServer server(io_service, 40000, std::cerr, std::cerr);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}


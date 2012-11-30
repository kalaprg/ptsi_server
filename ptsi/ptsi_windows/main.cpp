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
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        signals.async_wait(
                    boost::bind(&boost::asio::io_service::stop, &io_service));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}


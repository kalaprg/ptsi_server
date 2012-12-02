#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "ptsiserver.h"

using namespace std;

int main(int argc, char* argv[])
{
    std::string configFile;
    boost::program_options::options_description cmdlineOptions("Available options");
    cmdlineOptions.add_options()
            ("help,h", "produce this help message")
            ("config,c", boost::program_options::value<std::string>(&configFile)->default_value("ptsi.cfg"), "config file");

    boost::program_options::variables_map vm;
    try
    {
        boost::program_options::store(
                    boost::program_options::parse_command_line(argc, argv, cmdlineOptions), vm);
        boost::program_options::notify(vm);
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Usage: ptsi_windows [options]" << std::endl;
        std::cerr << cmdlineOptions << std::endl;
        globalOptions.printUsage(std::cerr);
        return EXIT_FAILURE;
    }
    if(vm.count("help"))
    {
        std::cerr << "Usage: ptsi_windows [options]" << std::endl;
        std::cerr << cmdlineOptions << std::endl;
        globalOptions.printUsage(std::cerr);
        return EXIT_SUCCESS;
    }

    if(!globalOptions.parseConfigFile(configFile))
        return EXIT_FAILURE;

    try
    {
        boost::asio::io_service io_service;
        PTSIServer server(io_service, std::cerr, std::cerr);
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        signals.async_wait(
                    boost::bind(&boost::asio::io_service::stop, &io_service));

        io_service.run();
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}


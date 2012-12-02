#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "common.h"
#include "ptsiserver.h"
#include "daemon.h"

const char Daemon::pidFileName_[] = "/var/run/ptsi_daemon.pid";

Daemon::Daemon()
    : lsyslog_((char *)"ptsi_daemon", LOG_INFO),
      lerrlog_((char *)"ptsi_daemon", LOG_ERR),
      ldbglog_((char *)"ptsi_daemon", LOG_DEBUG), started_(false)
{
}

Daemon::~Daemon()
{
    if(started_)
    {
        cleanPID();
        lsyslog_ << "ptsi_daemon stopped" << std::endl;
    }
}

int Daemon::start(int argc, char **argv)
{
    std::string configFile;
    boost::program_options::options_description cmdlineOptions("Available options");
    cmdlineOptions.add_options()
            ("help,h", "produce this help message")
            ("stop,s", "stops daemon")
            ("restart,r", "restarts daemon")
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
        std::cerr << "Usage: ptsi_daemon [options]" << std::endl;
        std::cerr << cmdlineOptions << std::endl;
        globalOptions.printUsage(std::cerr);
        return EXIT_FAILURE;
    }
    if(vm.count("help"))
    {
        std::cerr << "Usage: ptsi_daemon [options]" << std::endl;
        std::cerr << cmdlineOptions << std::endl;
        globalOptions.printUsage(std::cerr);
        return EXIT_SUCCESS;
    }

    if(vm.count("stop"))
    {
        killDaemon();
        return EXIT_SUCCESS;
    }

    if(!globalOptions.parseConfigFile(configFile))
        return EXIT_FAILURE;

    if(vm.count("restart"))
    {
        if(!killDaemon())
            return EXIT_FAILURE;
    }

    if(readPID())
    {
        std::cerr << "Another instance is already running. Use -r option."
                  << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        boost::asio::io_service io_service;

        // Initialise the server before becoming a daemon. If the process is
        // started from a shell, this means any errors will be reported back to the
        // user.
        PTSIServer server(io_service, lerrlog_, ldbglog_);

        // Register signal handlers so that the daemon may be shut down. You may
        // also want to register for other signals, such as SIGHUP to trigger a
        // re-read of a configuration file.
        boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
        signals.async_wait(
                    boost::bind(&boost::asio::io_service::stop, &io_service));

        // Inform the io_service that we are about to become a daemon. The
        // io_service cleans up any internal resources, such as threads, that may
        // interfere with forking.
        io_service.notify_fork(boost::asio::io_service::fork_prepare);
        // Fork the process and have the parent exit. If the process was started
        // from a shell, this returns control to the user. Forking a new process is
        // also a prerequisite for the subsequent call to setsid().
        if (pid_t pid = fork())
        {
            if (pid > 0)
            {
                // We're in the parent process and need to exit.
                //
                // When the exit() function is used, the program terminates without
                // invoking local variables' destructors. Only global variables are
                // destroyed. As the io_service object is a local variable, this means
                // we do not have to call:
                //
                //   io_service.notify_fork(boost::asio::io_service::fork_parent);
                //
                // However, this line should be added before each call to exit() if
                // using a global io_service object. An additional call:
                //
                //   io_service.notify_fork(boost::asio::io_service::fork_prepare);
                //
                // should also precede the second fork().
                return EXIT_SUCCESS;
            }
            else
            {
                lsyslog_ << logstream::setDebugLevel(LOG_ERR)
                         << "First fork failed:" << strerror(errno) << std::endl;
                return EXIT_FAILURE;
            }
        }

        // Make the process a new session leader. This detaches it from the
        // terminal.
        setsid();

        // A process inherits its working directory from its parent. This could be
        // on a mounted filesystem, which means that the running daemon would
        // prevent this filesystem from being unmounted. Changing to the root
        // directory avoids this problem.
        if(chdir("/"))
        {
            lsyslog_ << logstream::setDebugLevel(LOG_ERR)
                     << "Cannot change current directory:" << strerror(errno)
                     << std::endl;
            return EXIT_FAILURE;
        }

        // The file mode creation mask is also inherited from the parent process.
        // We don't want to restrict the permissions on files created by the
        // daemon, so the mask is cleared.
        umask(0);

        // A second fork ensures the process cannot acquire a controlling terminal.
        io_service.notify_fork(boost::asio::io_service::fork_prepare);
        if (pid_t pid = fork())
        {
            if (pid > 0)
            {
                return EXIT_SUCCESS;
            }
            else
            {
                lsyslog_ << logstream::setDebugLevel(LOG_ERR)
                         << "Second fork failed:" << strerror(errno) << std::endl;
                return EXIT_FAILURE;
            }
        }

        // Close the standard streams. This decouples the daemon from the terminal
        // that started it.
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        // We don't want the daemon to have any standard input.
        if (open("/dev/null", O_RDONLY) < 0)
        {
            lsyslog_ << logstream::setDebugLevel(LOG_ERR)
                     << "Unable to open /dev/null:" << strerror(errno) << std::endl;
            return EXIT_FAILURE;
        }

        // Inform the io_service that we have finished becoming a daemon. The
        // io_service uses this opportunity to create any internal file descriptors
        // that need to be private to the new process.
        io_service.notify_fork(boost::asio::io_service::fork_child);

        started_ = true;
        writePID();
        lsyslog_ << "ptsi_daemon started" << std::endl;

        io_service.run();
    }
    catch (std::exception& e)
    {
        lsyslog_ << logstream::setDebugLevel(LOG_ERR)
                 << "Exception: " << e.what() << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool Daemon::killDaemon() const
{
    pid_t pid = readPID();
    if(!pid)
        return true;

    if(kill(pid, SIGTERM))
    {
        std::cerr << "Unable to kill previous instance of daemon: "
                  << strerror(errno) << std::endl;
        return false;
    }

    //wait until daemon terminates
    int tries = 10;
    while(kill(pid, 0) != -1 && errno != ESRCH && tries--)
        usleep(100);

    if(!tries)
        std::cerr << "Previous instance is not responding" << std::endl;

    return tries;
}

pid_t Daemon::readPID() const
{
    std::ifstream pidFile(pidFileName_);
    if(pidFile)
    {
        pid_t pid;
        pidFile >> pid;
        return pid;
    }
    else
        return 0;
}

void Daemon::cleanPID() const
{
    unlink(pidFileName_);
}

void Daemon::writePID() const
{
    pid_t pid = getpid();
    std::ofstream pidFile(pidFileName_);
    pidFile << pid;
}

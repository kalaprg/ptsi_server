#ifndef DAEMON_H
#define DAEMON_H
#include <cstdio>
#include <logstream/log.h>

class Daemon
{
public:
    Daemon();
    ~Daemon();
    int start(int argc, char **argv);

private:
    bool killDaemon() const;
    pid_t readPID() const;
    void cleanPID() const;
    void writePID() const;
    logstream::log lsyslog_;
    logstream::log lerrlog_;
    logstream::log ldbglog_;
    bool started_;

    static const char pidFileName_[];
};

#endif // DAEMON_H

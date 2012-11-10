#include "daemon.h"

int main(int argc, char **argv)
{
    Daemon daemon;
    return daemon.start(argc, argv);
}

#include "commonFunctions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

bool killed;

void handle_signal(int signal)
{
    const char * signal_name;
    sigset_t pending;

    switch(signal)
    {
        case SIGTERM:
            killed = true;
            cleanUp();
            exit(0);
            break;
        case SIGINT:
            killed = true;
            cleanUp();
            exit(0);
            break;
        case SIGQUIT:
            killed = true;
            cleanUp();
            exit(0);
            break;
        default:
            fprintf(stderr,"Caught wrong signal: %d\n",signal);
            return;
    }
}

int createSocket(int domain, int type, int protocol)
{
    int welcomeSocket = socket(domain, type, protocol);
    if(welcomeSocket == -1)
    {
        perror("Couldn't create socket");
        exit(EXIT_FAILURE);
    }

    return welcomeSocket;
}




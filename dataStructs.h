#ifndef DATASTRUCTS_H_INCLUDED
#define DATASTRUCTS_H_INCLUDED

#define PORT 8080
#define MAX_NUM_CONNECTIONS 2
#include <pthread.h>
#include <stdbool.h>

struct message
{
    char to[MAX];
    char from[MAX];
    char * body;
};

struct chatRoom
{
	//Maximum number of threads and therefore connections allowed to server
	pthread_t threads[MAX_NUM_CONNECTIONS];
    struct clientInfo * threadsInfo[MAX_NUM_CONNECTIONS];
    char nameOfChatRoom[MAX];
    
};

struct clientInfo
{
	int socket;
	char buffer[MAX];
    char nameOfClient[MAX];
    struct chatRoom * chtRoom;
    bool connected;
    int threadIndexNum;
};

typedef struct message Message;
typedef struct clientInfo ClientInfo;
typedef struct chatRoom ChatRoom;
#endif //DATASTRUCTS_H_INCLUDED

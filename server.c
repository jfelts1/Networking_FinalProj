/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "fileUtil.h"
#include "dataStructs.h"
#include <pthread.h>
#include "commonFunctions.h"

//#define MAX 1024
#define MAX_NUM_CONNECTIONS 2

struct info
{
    int socket;
    char buffer[MAX];
    ClientInfo Cinfo;

    int threadIndexNum;
};

typedef struct info Info;

//Maximum number of threads and therefore connections allowed to server
pthread_t threads[MAX_NUM_CONNECTIONS];
Info * threadsInfo[MAX_NUM_CONNECTIONS];
pthread_mutex_t mutex;
extern bool killed;
int welcomeSocket;


void * ChatThread(void * arg);
int createSocket();
struct sockaddr_in configureSockaddr_in(int domain,int port,char * address);
int findFreeThreadSlot();
int recieveClientInfo(int socket, char * buffer);


int main()
{
    signal(SIGINT,handle_signal);
    signal(SIGQUIT,handle_signal);
    killed = false;
    int newSocket;
    int err = 0;
    char buffer[MAX] = "";
	
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    welcomeSocket = createSocket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    serverAddr = configureSockaddr_in(AF_INET,PORT,"127.0.0.1");

    /*---- Bind the address struct to the socket ----*/
    err = bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if(err == -1)
    {
        perror("Couldn't bind");
        close(welcomeSocket);
    }

    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,MAX_NUM_CONNECTIONS)==0)
        printf("Listening\n");
    else
        printf("Error\n");

    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    //initilize the array of threads to all 0
    int i = 0;
    for(i=0;i<MAX_NUM_CONNECTIONS;i++)
    {
        threads[i] = 0;
    }
    for(i=0;i<MAX_NUM_CONNECTIONS;i++)
    {
        threadsInfo[i]=NULL;
    }
	int recved = 0;
    while(true)
    {
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
        //receive client info
        recved = recieveClientInfo(newSocket,buffer);
        //block all access to the thread array while checking for free spots and adding
        //a thread to it
        pthread_mutex_lock(&mutex);
        int freeThread = findFreeThreadSlot();
        if(freeThread != -1)
        {
            printf("Connecting to %s\n",buffer);
            Info * data = malloc(sizeof(Info));

            //strcpy(data->buffer,buffer);
            data->socket = newSocket;
            data->threadIndexNum = freeThread;
            strcpy(data->Cinfo.nameOfClient,buffer);
            threadsInfo[freeThread] = data;

            pthread_create(&threads[freeThread],NULL,ChatThread,(void *)data);
        }
        else
        {
            puts("no room available");
            close(newSocket);
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}


void * ChatThread(void * arg)
{
    Info * data = (Info *)arg;

    char returnMessage[MAX] = "";
    printf("Connected to %s\n",data->Cinfo.nameOfClient);
    /*added by james felts*/
    int recved = 0;
    int sended = 0;
    bool connected = true;

    while(connected && killed == false)
    {
        recved = recv(data->socket,data->buffer,MAX,0);

        //scanf("%i",&recved);
        if(recved == -1)
        {
            perror("recv error");
        }
        printf("reveived from %s: %s",data->Cinfo.nameOfClient,data->buffer);
        /*---- Send message to the socket of the incoming connection ----*/

        int i = 0;
        for(i = 0;i<MAX_NUM_CONNECTIONS;i++)
        {
            if(threadsInfo[i]!=NULL)
            {
                sended = send(threadsInfo[i]->socket,data->buffer,MAX,0);
                printf("Send to %s\n",threadsInfo[i]->Cinfo.nameOfClient);
            }
        }


        //sended = send(data->socket,returnMessage,MAX,0);

        if(sended == -1)
        {
            perror("send error");
        }
    }
    return NULL;
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

struct sockaddr_in configureSockaddr_in(int domain,int port, char * address)
{
    struct sockaddr_in serverAddr;
    /* Address family = Internet */
    serverAddr.sin_family = domain;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(port);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(address);
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    return serverAddr;
}

int findFreeThreadSlot()
{
    int i = 0;
    for(i=0;i<MAX;i++)
    {
        if(threads[i]==0)
        {
            return i;
        }
    }

    //no free threads
    return -1;
}

int recieveClientInfo(int socket, char * buffer)
{
    int recved = 0;
    recved = recv(socket,buffer,MAX,0);
    if(recved == -1)
    {
        perror("Recieve error");
    }
    return recved;
}

void cleanUp()
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    for(i = 0;i<MAX_NUM_CONNECTIONS;i++)
    {
        if(threadsInfo[i]!=NULL)
        {
            close(threadsInfo[i]->socket);
            free(threadsInfo[i]);
        }
    }
    pthread_mutex_unlock(&mutex);
    close(welcomeSocket);
    puts("Cleaned up");
}




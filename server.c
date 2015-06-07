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

ChatRoom * defaultChatRoom;
pthread_mutex_t mutex;
pthread_t zombieKillingThread;
extern bool killed;
int welcomeSocket;


void * ChatThread(void * arg);
void * KillZombieThreadsThread(void * arg);

struct sockaddr_in configureSockaddr_in(int domain,int port,char * address);
int findFreeThreadSlot();
int recieveClientInfo(int socket, char * buffer);
void handle_client_exit(int signal);
void sendToAllConnectedClients(char * message, ClientInfo * data);



int main()
{
    signal(SIGINT,handle_signal);
    signal(SIGQUIT,handle_signal);
    signal(SIGPIPE,handle_client_exit);
    defaultChatRoom = (ChatRoom *)malloc(sizeof(ChatRoom));
    char defaultChatRoomName[MAX] = "default chat room";
    memcpy(defaultChatRoom->nameOfChatRoom,defaultChatRoomName,strlen(defaultChatRoomName)+1);
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
        defaultChatRoom->threads[i] = 0;
    }
    for(i=0;i<MAX_NUM_CONNECTIONS;i++)
    {
        defaultChatRoom->threadsInfo[i]=NULL;
    }
	int recved = 0;
	pthread_create(&zombieKillingThread,NULL,KillZombieThreadsThread,NULL);
    while(true)
    {
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
        //receive client info
        recved = recieveClientInfo(newSocket,buffer);
        strip(buffer);
        //block all access to the thread array while checking for free spots and adding
        //a thread to it
        pthread_mutex_lock(&mutex);
        int freeThread = findFreeThreadSlot();
        if(freeThread != -1)
        {
            printf("Connecting to %s\n",buffer);
            ClientInfo * data = malloc(sizeof(ClientInfo));

            //strcpy(data->buffer,buffer);
            data->socket = newSocket;
            data->threadIndexNum = freeThread;
            data->connected = true;
            strcpy(data->nameOfClient,buffer);
            defaultChatRoom->threadsInfo[freeThread] = data;

            pthread_create(&(defaultChatRoom->threads[freeThread]),NULL,ChatThread,(void *)data);
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
    ClientInfo * data = (ClientInfo *)arg;

    char returnMessage[MAX] = "";
    printf("Connected to %s\n",data->nameOfClient);
    int recved = 0;
    int sended = 0;
    int nameLen = strlen(data->nameOfClient);
    bool connected = true;
    char sendBuffer[MAX+MAX] = "";
    char tempBuf[MAX] = "";
	
    while(connected && killed == false)
    {
        recved = recv(data->socket,data->buffer,MAX,0);
        if(strcmp("/quit",data->buffer)==0)
        {
			int len = strlen(data->nameOfClient);
			memcpy(tempBuf,data->nameOfClient,len);
			memcpy(tempBuf+len," quit the room\n",strlen(" quit the room\n")+1);
			printf("%s",tempBuf);
			sendToAllConnectedClients(tempBuf,data);
			
			close(data->socket);
			data->connected = false;
			return NULL;
		}
		
		memcpy(sendBuffer,data->nameOfClient,nameLen);
		memcpy(sendBuffer+nameLen,": ",2);
		memcpy(sendBuffer+nameLen+2,data->buffer,strlen(data->buffer)+1);
        //scanf("%i",&recved);
        if(recved == -1)
        {
            perror("recv error");
        }
        printf("reveived from %s: %s",data->nameOfClient,data->buffer);
        /*---- Send message to the socket of the incoming connection ----*/
		sendToAllConnectedClients(sendBuffer,data);
        


        //sended = send(data->socket,returnMessage,MAX,0);

        
    }
    return NULL;
}

void * KillZombieThreadsThread(void * arg)
{
	while(true)
	{
		sleep(1);
		int i = 0;
		pthread_mutex_lock(&mutex);
		for(i=0;i<MAX_NUM_CONNECTIONS;i++)
		{
			if(defaultChatRoom->threadsInfo[i]!=NULL)
			{
				if(defaultChatRoom->threadsInfo[i]->connected == false)
				{
					pthread_join(defaultChatRoom->threads[i],NULL);
					defaultChatRoom->threads[i]=0;
					free(defaultChatRoom->threadsInfo[i]);
					defaultChatRoom->threadsInfo[i]=NULL;
				}
			}
		}
		pthread_mutex_unlock(&mutex);
		
	}
	return NULL;
}

void sendToAllConnectedClients(char * message, ClientInfo * data)
{
	int i = 0;
	int sended = 0;
    for(i = 0;i<MAX_NUM_CONNECTIONS;i++)
    {
        if(defaultChatRoom->threadsInfo[i]!=NULL)
        {
			if(strcmp(defaultChatRoom->threadsInfo[i]->nameOfClient,data->nameOfClient) != 0 && defaultChatRoom->threadsInfo[i]->connected == true)
			{
				sended = send(defaultChatRoom->threadsInfo[i]->socket,message,MAX,0);
				//printf("Send to %s\n",threadsInfo[i]->Cinfo.nameOfClient);
			}
                
        }
    }
    
    if(sended == -1)
    {
        perror("send error");
    }
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
    for(i=0;i<MAX_NUM_CONNECTIONS;i++)
    {
        if(defaultChatRoom->threads[i]==0)
        {
			printf("thread index:%i\n",i);
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
        if(defaultChatRoom->threadsInfo[i]!=NULL)
        {
            close(defaultChatRoom->threadsInfo[i]->socket);
            free(defaultChatRoom->threadsInfo[i]);
        }
    }
    pthread_mutex_unlock(&mutex);
    close(welcomeSocket);
    free(defaultChatRoom);
    puts("Cleaned up");
}

void handle_client_exit(int signal)
{
	if(signal == SIGPIPE)
	{
		
	}
}




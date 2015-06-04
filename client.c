/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "fileUtil.h"
#include "dataStructs.h"
#include <pthread.h>
#include <stdbool.h>
#include "commonFunctions.h"

void * recieveThread(void * arg);
extern bool killed;
int clientSocket;

int main()
{
	  signal(SIGINT,handle_signal);
      signal(SIGQUIT,handle_signal);
      killed = false;
      pthread_t thread;
      char buffer[MAX] = "";
      struct sockaddr_in serverAddr;
      socklen_t addr_size;

      /*---- Create the socket. The three arguments are: ----*/
      /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
      clientSocket = socket(PF_INET, SOCK_STREAM, 0);

      /*---- Configure settings of the server address struct ----*/
      /* Address family = Internet */
      serverAddr.sin_family = AF_INET;
      /* Set port number, using htons function to use proper byte order */
      serverAddr.sin_port = htons(PORT);
      /* Set IP address to localhost */
      serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
      /* Set all bits of the padding field to 0 */
      memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

      /*---- Connect the socket to the server using the address struct ----*/
      addr_size = sizeof serverAddr;
      connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
      puts("Enter name");
      fgets(buffer,MAX,stdin);
      send(clientSocket,buffer,MAX,0);
      int * clientSocketPtr = malloc(sizeof(int));
      *clientSocketPtr = clientSocket;
      pthread_create(&thread,NULL,recieveThread,clientSocketPtr);
      /*added by james felts*/
      while(strcmp(buffer,"/quit\n")!=0)
      {
            puts("Enter message");
            fgets(buffer,MAX,stdin);
            send(clientSocket,buffer,MAX,0);
      }
      cleanUp();

      return 0;
}

void * recieveThread(void * arg)
{
    int clientSocket = (int)*((int *)arg);
    char buffer[MAX] = "";
    while(true)
    {
        /*---- Read the message from the server into the buffer ----*/
        recv(clientSocket, buffer, MAX, 0);

        /*---- Print the received message ----*/
        printf("%s",buffer);
    }


    return NULL;
}

void cleanUp()
{
	send(clientSocket,"/quit",MAX,0);
	puts("Quiting chat room");
	close(clientSocket);
	puts("CleanedUp");
}

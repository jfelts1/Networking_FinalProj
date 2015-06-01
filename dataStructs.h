#ifndef DATASTRUCTS_H_INCLUDED
#define DATASTRUCTS_H_INCLUDED

#define PORT 8080

struct message
{
    char to[MAX];
    char from[MAX];
    char * body;
};

typedef struct message Message;

struct clientInfo
{
    char nameOfClient[MAX];
};
typedef struct clientInfo ClientInfo;

#endif //DATASTRUCTS_H_INCLUDED

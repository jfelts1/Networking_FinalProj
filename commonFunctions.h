#ifndef COMMONFUNCTIONS_H_INCLUDED
#define COMMONFUNCTIONS_H_INCLUDED
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
extern bool killed;

//using this function REQUIRES you to write a function named cleanUp()
void handle_signal(int signal);
extern void cleanUp();

#endif //COMMONFUNCTIONS_H_INCLUDED

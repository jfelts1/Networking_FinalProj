#ifndef FILEUTIL_H_INCLUDED
#define FILEUTIL_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024
#define CLEARBUFF while(fgetc(stdin)!='\n');

/*Opens input file from program arguments*/
FILE * openInputFile_Args(int,char **);
/*Opens input file from string passed in*/
FILE * openInputFile_String(char *);
/*Opens input file after prompting the user for a filename*/
FILE * openInputFile_Prompt();
/*Same thing as openInputFile_Prompt*/
FILE * openFile();

/*Opens/creates an output file from of the name passed in*/
FILE * openOutputFile_FileName(char *);
/*Opens/creates and output file after prompting the user for a filename*/
FILE * openOutputFile_Prompt();

/*Counts the number of records in a file*/
int countRecords(FILE *, int);

/*Prompts the user for the name of a file and returns the string*/
char * readFileName();

/*displays the number of lines to stdout*/
void displayFile_TotalLines(int, FILE *);
/*displays the contents of the file passed in to stdout*/
void displayFile_FilePointer(FILE *);
/*Writes the contents of the first file passed into the second file passed in*/
void displayFile_OutputFile(FILE *, FILE *);

/*removes the carriage return from the string passed in*/
void strip(char *);


#endif // FILEUTIL_H_INCLUDED

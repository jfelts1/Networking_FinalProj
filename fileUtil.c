#include "fileUtil.h"

/*James Felts*/

FILE * openInputFile_Args(int argc, char * argv[])
{
    FILE * fin = NULL;
    char * fn = NULL;

    /*If insufficient arguments exit the program*/
    if(argc < 2)
    {
        exit(-1);
    }

    fn = argv[1];
    fin = fopen(fn, "r");

    while(fin == NULL)
    {
        char temp[MAX];
        CLEARBUFF
        puts("File did not open.");
        puts("Please enter the name of an input file :");
        fgets(temp,MAX,stdin);
        strip(temp);

        fin =fopen(temp,"r");
    }

    return fin;
}//end openInputFile_Args

FILE * openInputFile_String(char * fileName)
{
    strip(fileName);
    FILE * fin = fopen(fileName,"r");

    while(fin == NULL)
    {
        char temp[MAX];
        CLEARBUFF
        puts("File did not open.");
        puts("Please enter the name of an input file :");
        fgets(temp,MAX,stdin);
        strip(temp);

        fin =fopen(temp,"r");
    }

    return fin;
}//end openInputFile_String

FILE * openInputFile_Prompt()
{
    FILE * fin = NULL;
    char fn[MAX];

    puts("Enter the name of the input file:");
    fgets(fn,MAX,stdin);
    strip(fn);

    fin = fopen(fn,"r");

    while(fin == NULL)
    {
        char temp[MAX];
        CLEARBUFF
        puts("File did not open.");
        puts("Please enter the name of an input file :");
        fgets(temp,MAX,stdin);
        strip(temp);

        fin =fopen(temp,"r");
    }

    return fin;
}//end openInputFile_Prompt

FILE * openFile()
{
    FILE * fin = NULL;
    char fn[MAX];

    puts("Enter the name of the input file:");
    fgets(fn,MAX,stdin);
    strip(fn);

    fin = fopen(fn,"r");

    while(fin == NULL)
    {
        char temp[MAX];
        CLEARBUFF
        puts("File did not open.");
        puts("Please enter the name of an input file :");
        fgets(temp,MAX,stdin);
        strip(temp);

        fin =fopen(temp,"r");
    }

    return fin;
}//end openInputFile_Prompt

FILE * openOutputFile_FileName(char * fileName)
{
    FILE * fout = NULL;
    strip(fileName);

    fout = fopen(fileName, "w");

    return fout;
}//end openOutputFile_FileName

FILE * openOutputFile_Prompt()
{
    FILE * fout = NULL;
    char fileName[MAX];

    puts("Enter the name of the output file: ");
    fgets(fileName,MAX,stdin);
    strip(fileName);

    fout = fopen(fileName,"w");
    return fout;
}//end openOutputFile_Prompt

int countRecords(FILE * fin, int linesPerRecord)
{
    int count = 0;
    char temp[MAX];

    fgets(temp,MAX,fin);

    while(!feof(fin))
    {
        count++;
        fgets(temp,MAX,fin);
    }
    return count/linesPerRecord;
}//end countRecords

char * readFileName()
{
    char fileName[MAX];
    char * fn;
    puts("Enter the name of a file:");

    fgets(fileName,MAX,stdin);
    strip(fileName);

    fn = (char *)calloc((strlen(fileName)+1),sizeof(char));
    strcpy(fn,fileName);

    return fn;
}//end readFileName

void displayFile_TotalLines(int records, FILE * fin)
{
    int count = 0;
    char temp[MAX];

    fgets(temp,MAX,fin);

    while(!feof(fin))
    {
        count++;
        fgets(temp,MAX,fin);
    }

    printf("File has %i records over %i lines.\n",records,count);
}//end displayFile_TotalLines

void displayFile_FilePointer(FILE * fin)
{
    char temp[MAX];

    fgets(temp,MAX,fin);
    puts(temp);
    while(!feof(fin))
    {
        fgets(temp,MAX,fin);
        puts(temp);
    }
}//end displayFile_FilePointer

void displayFile_OutputFile(FILE * fin, FILE * fout)
{
    char temp[MAX];

    fgets(temp,MAX,fin);
    fprintf(fout,"%s\n",temp);
    while(!feof(fin))
    {
        fgets(temp,MAX,fin);
        fprintf(fout,"%s\n",temp);
    }
}//end displayFile_OutputFile

void strip(char * string)
{
    int len = (int)strlen(string);
    int i;

    for(i = 0; i < len;i++)
    {
        if(string[i]=='\r' || string[i]=='\n')
        {
            string[i] = '\0';
        }
    }
}//end strip

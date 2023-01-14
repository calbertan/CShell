#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

// start of environment variable //
#define envArrSize 1000
int currentEnvArrSize = 0;

typedef struct
{
    char *name;
    char *value;
    
} EnvVar;

EnvVar envArr[envArrSize];
pid_t pid;
int argi;
char *command[256];

short getEnvArrIndex(char *str)
{   
    //printf("%d\n",currentEnvArrSize);
    for (int i = 0; i < currentEnvArrSize; i++)
    {   
        //printf("%s",envArr[i].name);
        if (strcmp(envArr[i].name, str) == 0)
            return i;
    }
    return -1;
}

int checkForEnvVar(char *str)
{
    if (str == NULL || strlen(str) == 0)
    {
        return 1;
    }
    char *context = NULL;
    char *test = strtok_r(str, " ", &context);
    // used strtok_r to avoid conflict with strtok
    while (test != NULL)
    {
        
        char *found = strstr(test, "\n");
        if (found != NULL)
        {
            *found = '\0';
        }
        if (test[0] == '$' && getEnvArrIndex(test + 1) == -1)
        {
            return 0;
        }
        test = strtok_r(NULL, " ", &context);
    }
    return 1;
}
char *substring(char *string, int position, int length)
{
   char *p;
   int c;
 
   p = (char *)malloc(sizeof(char) * (length-position+1));
 
   for (c = 0; c < length; c++)
   {
      *(p+c) = *(string+position-1);      
      string++;  
   }
 
   *(p+c) = '\0';
 
   return p;
}
void printCommand(char *str)
{
    if (str == NULL)
        return;
    char *found = strstr(str, "\n");
    if (found != NULL)
    {
        *found = '\0';
    }

    if (str[0] == '$')
    {
        for (int i = 0; i < currentEnvArrSize; i++)
        {
            if (strcmp(envArr[i].name, str + 1) == 0)
            {
                printf("%s ", envArr[i].value);
                break;
            }
        }
    }
    else
    {
        printf("%s ", str);
    }
}

void printAllEnvArr()
{
    for (int i = 0; i < currentEnvArrSize; i++)
    {
        printf("%s=%s\n", envArr[i].name, envArr[i].value);
    }
}

void freeAllEnvArr()
{
    for (int i = 0; i < currentEnvArrSize; i++)
    {
        free(envArr[i].name);
        free(envArr[i].value);
    }
}
// end of environment variable //
#define logSize 1000
int currentLogSize = 0;

typedef struct
{
    char *name;
    struct tm **time;
    int *returnValue;
} Command;

Command logArr[logSize];

void printAllLog()
{
    for (int i = 0; i < currentLogSize; i++)
    {
        printf("%s %s %d\n", asctime(*(logArr[i].time)), logArr[i].name, *(logArr[i].returnValue));
    }
}

void freeAllLog()
{
    for (int i = 0; i < currentLogSize; i++)
    {
        free(logArr[i].time);
        free(logArr[i].name);
        free(logArr[i].returnValue);
    }
}

FILE *file;
// FILE *fileToBeFreed;
short fileExists;

int main(int argc, char *argv[])
{
    short notExit = 1;
    int code = 0;

    char line[2048];
    char lineTemp[2048];

    fileExists = 0;
    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == 0)
        {
            printf("Could not open file\n");
            return 0;
        }
        else
        {
            // fileToBeFreed = file;
            fileExists = 1;
        }
    }
    while (notExit)
    {
        if (fileExists)
        {
            if (fgets(line, 1024, file) != NULL)
            {
                line[strlen(line)-1]='\0';
                for (int i = strlen(line) - 1; i >= 0; i--) {
                    if (line[i] == '\r') {
                        line[i] = '\0';
                    }
                }
            }
            else
            {
                notExit = 0;
                break;
            }
        }
        else
        {
            printf("cshell$ ");
            fgets(line, 1024, stdin);
            // remove trailing newline
            if ((strlen(line) > 0) && (line[strlen (line) - 1] == '\n'))
        	    line[strlen (line) - 1] = '\0';

     

        }
        strcpy(lineTemp, line);
        char *cptr;
        argi=0;
        cptr = strtok(line, " ");
        code=0;

        while(cptr!=NULL){
            command[argi]=cptr;
            argi++;
            cptr=strtok(NULL," ");
        }

        if(!strcmp("&", command[argi-1])){
            command[argi-1] = NULL;
            command[argi] = "&";
        }else{
            command[argi] = NULL;
        }

        char *currentCommand = (char *)malloc(sizeof(char) * strlen(command[0]));
        strcpy(currentCommand, command[0]);
        logArr[currentLogSize].name = currentCommand;

        if (strcmp(command[0], "exit") == 0)
        {
            printf("Bye!\n");
            notExit = 0;
        }
        else if (strcmp(command[0], "theme") == 0)
        {
            if (command[1] == NULL || strcmp(command[1], "\n") == 0)
            {
                printf("Error: no argument found\n");
            }
            else
            {        
                if (strcmp(command[1], "red") == 0)
                {
                    printf("\033[0;31m");
                }
                else if (strcmp(command[1], "green") == 0)
                {
                    printf("\033[0;32m");
                }
                else if (strcmp(command[1], "blue") == 0)
                {
                    printf("\033[0;34m");

                }
                else
                {
                    printf("unsupported theme\n");
                }
            }
        }
        else if (strcmp(command[0], "print") == 0)
        {
            if (command[1] == NULL || strcmp(command[1], "\n") == 0)
            {
                printf("Error: no argument found\n");
            }
            else
            {
                // printAllEnvArr();
                if (!checkForEnvVar(lineTemp))
                {
                    printf("Error: Variable not defined\n");
                }
                else
                {
                    for(int i=1; i<argi;i++)
                    {
                        printCommand(command[i]);
                    }
                    printf("\n");
                }
            }
        }
        else if (command[0][0] == '$')
        {   
            
                char *varname;
                char *varval;
                int index = 0;
                int equalSign = 0;

                for (int i = 1; i < strlen(command[0]); i++)
                {   
                    if(command[0][i]=='=')
                    {   
                        if(i==1) printf("Variable value expected\n");   //$=
                        equalSign = 1;
                        index = i;
                        break;
                    }
                }
                if(!equalSign)
                {
                    printf("Variable value expected\n");
                }
                else
                {   
                    varname = substring(lineTemp, 2, index-1);
                    varval = substring(lineTemp, index + 2, strlen(lineTemp));      //varval = everything after =
                    // printf("varname: %s\n",varname);
                    // printf("varval: %s\n",varval);
                    char *trueName = (char *)malloc(sizeof(char) * (strlen(varname)+1));
                    char *trueVal = (char *)malloc(sizeof(char) * (strlen(varval+1)));
                    for (int i = 0; i < strlen(varname); i++)
                    {
                        trueName[i] = varname[i];
                    }
                    trueName[strlen(varname)] = '\0';
                    for (int i = 0; i < strlen(varval); i++)
                    {
                        trueVal[i] = varval[i];
                    }
                    trueVal[strlen(varval)] = '\0';
                    

                    short indexOfName = getEnvArrIndex(trueName);
                    if (indexOfName == -1)
                    {
                        //printf("NEW\n");
                        // key is missing
                        envArr[currentEnvArrSize].name = trueName;
                        envArr[currentEnvArrSize].value = trueVal;
                        currentEnvArrSize++;
                    }
                    else
                    {
                        //printf("OLD\n");
                        // key already exists, just change value
                        free(envArr[indexOfName].value);
                        envArr[indexOfName].value = trueVal;
                    }
                    free(varname);
                    free(varval);

                    // printf("%s",envArr[currentEnvArrSize-1].name);
                    // printf("%s",envArr[currentEnvArrSize-1].value);
                }

            
        }
        else if (strcmp(command[0], "log") == 0)
        {
            if (!(command[1] == NULL || strcmp(command[1], "\n") == 0))
            {
                printf("Error: Too many Arguments detected\n");
            }
            else
            {
                printAllLog();
            }
        } else {
			pid = fork();
			if(-1 == pid){
				printf("failed to create a child\n");
			}
			else if(0 == pid){
                // printf("command: |%s|\n", command[0]);
                //printf("%s",command[1]);
				//printf("hello from child\n");
				// execute a command
                if(strcmp(command[0],"ls")==0){
                    command[1] = "-1";
                    command[2] = NULL;
                    // command[1][2] = "\0";
                }
                // printf("command 1:|%s|\n", command[1]);
                // printf("command 2:|%s|\n", command[2]);

                if(execvp(command[0], command)==-1){
                    printf("Missing keyword or command, or permission problem\n");
                    code = 1;
                    notExit = 0;
                }
			}
			else{
				//printf("hello from parent\n");
				// wait for the command to finish if "&" is not present
				if(NULL == command[argi]) waitpid(pid, NULL, 0);
			}
        }
        struct tm **ptr;
        time_t t;
        t = time(NULL);
        ptr = (struct tm **)malloc(sizeof(struct tm *));
        *ptr = localtime(&t);
        logArr[currentLogSize].time = ptr;

        int *currentRetVal = (int *)malloc(sizeof(int));
        *currentRetVal = code;
        logArr[currentLogSize++].returnValue = currentRetVal;
        
    }
    freeAllEnvArr();
    freeAllLog();
    if (fileExists) {
    fclose(file);
    }


}

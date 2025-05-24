#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    if (argc > 2) errx(1, "Error");

    const char* command = (argc == 2) ? argv[1] : "echo";
    if (strlen(command) > 4) errx(1, "Error");

    char* arguments[4]; 
    arguments[0] = (char*)command;
    int countOfArguments = 0;

    char currentWord[6]; 
    int currentPosition = 0;
    char currentSymbol = 'a';

    while (read(0, &currentSymbol, 1) > 0)
    {

        if (currentSymbol == ' ' || currentSymbol == '\n') 
        {

            if (currentPosition == 0) continue; 
            currentWord[currentPosition] = '\0';
            if (strlen(currentWord) > 4) errx(1, "Error");
                

            arguments[countOfArguments + 1] = strdup(currentWord);
            countOfArguments += 1;
            currentPosition = 0;

            if (countOfArguments == 2) 
            {

                arguments[3] = NULL;
                pid_t currentPid = fork();

                if (currentPid == 0)
                {

                    execvp(command, arguments);
                    err(1, "Error");

                }

                int status = 0;
                wait(&status);
                if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) errx(1, "Error");
                    
                free(arguments[1]);
                free(arguments[2]);
                countOfArguments = 0;

            }
        } 
        else 
        {

            if (currentPosition >= 4) errx(1, "Error");
            currentWord[currentPosition] = currentSymbol;
            currentPosition += 1;

        }

    }

    if (currentPosition > 0) 
    {

        currentWord[currentPosition] = '\0';
        if (strlen(currentWord) > 4) errx(1, "Error");
            
        arguments[1] = strdup(currentWord);
        arguments[2] = NULL;
        pid_t currentPid = fork();

        if (currentPid == 0)
        {

            execvp(command, arguments);
            err(1, "Error");

        }

        int status = 0;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) errx(1, "Error");
            
        free(arguments[1]);

    }

    return 0;

}


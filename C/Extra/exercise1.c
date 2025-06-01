#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{

    if(argc != 3) errx(1, "Error");

    int counter = 0;
    const char terminatingSymbol = '$';
    const char seprator = '\n';

    const char* commandForEven = argv[1];
    const char* commandForOdd = argv[2];
    int countOfEvenArgs = 0;
    int countOfOddArgs = 0;

    int sharedDataForEven[2];
    int sharedDataForOdd[2];
    pipe(sharedDataForEven);
    pipe(sharedDataForOdd);

    while(true)
    {

        char buffer[100];
        int readBytes = read(0, buffer, sizeof(buffer));
        buffer[readBytes] = '\0';

        if(readBytes < 0) 
        {

            close(sharedDataForEven[0]);
            close(sharedDataForEven[1]);
            close(sharedDataForOdd[0]);
            close(sharedDataForOdd[1]);
            err(1, "Error");

        }

        if (readBytes > 0 && buffer[0] == terminatingSymbol) break;

        if(counter % 2 == 0)
        {

            if(write(sharedDataForEven[1], buffer, strlen(buffer)) < 0 ||
                write(sharedDataForEven[1], &seprator, sizeof(char)) < 0)
            {

                close(sharedDataForEven[0]);
                close(sharedDataForEven[1]);
                close(sharedDataForOdd[0]);
                close(sharedDataForOdd[1]);
                err(1, "Error");

            }

            countOfEvenArgs += 1;

        }
        else
        {

            if(write(sharedDataForOdd[1], buffer, strlen(buffer)) < 0 ||
                write(sharedDataForOdd[1], &seprator, sizeof(char)) < 0)
            {

                close(sharedDataForEven[0]);
                close(sharedDataForEven[1]);
                close(sharedDataForOdd[0]);
                close(sharedDataForOdd[1]);
                err(1, "Error");

            }

            countOfOddArgs += 1;

        }

        counter += 1;

    }

    close(sharedDataForEven[1]);
    close(sharedDataForOdd[1]);

    pid_t pidForEven = fork();

    if(pidForEven < 0)
    {

        close(sharedDataForEven[0]);
        close(sharedDataForEven[1]);
        close(sharedDataForOdd[0]);
        close(sharedDataForOdd[1]);
        err(1, "Error");

    }

    if (pidForEven == 0) 
    {
    
        close(sharedDataForEven[1]);
        dup2(sharedDataForEven[0], STDIN_FILENO);
        close(sharedDataForEven[0]);

        execlp("xargs", "xargs", commandForEven, (char*) NULL);
        err(1, "Error");

    }

    pid_t pidForOdd = fork();

    if(pidForOdd < 0)
    {

        close(sharedDataForEven[0]);
        close(sharedDataForEven[1]);
        close(sharedDataForOdd[0]);
        close(sharedDataForOdd[1]);
        err(1, "Error");

    }

    if (pidForOdd == 0) 
    {

        close(sharedDataForOdd[1]);
        dup2(sharedDataForOdd[0], STDIN_FILENO);
        close(sharedDataForOdd[0]);

        execlp("xargs", "xargs", commandForOdd, (char*) NULL);
        err(1, "Error");

    }

    int sharedStatusVariable = 0;

    if(waitpid(pidForEven, &sharedStatusVariable, 0) < 0)
    {

        close(sharedDataForEven[0]);
        close(sharedDataForEven[1]);
        close(sharedDataForOdd[0]);
        close(sharedDataForOdd[1]);
        err(1, "Error");

    }

    if(!WIFEXITED(sharedStatusVariable) || WEXITSTATUS(sharedStatusVariable) != 0)
    {

        close(sharedDataForEven[0]);
        close(sharedDataForEven[1]);
        close(sharedDataForOdd[0]);
        close(sharedDataForOdd[1]);
        err(1, "Error");

    }

    if(waitpid(pidForOdd, &sharedStatusVariable, 0) < 0)
    {

        close(sharedDataForEven[0]);
        close(sharedDataForEven[1]);
        close(sharedDataForOdd[0]);
        close(sharedDataForOdd[1]);
        err(1, "Error");

    }

    if(!WIFEXITED(sharedStatusVariable) || WEXITSTATUS(sharedStatusVariable) != 0)
    {

        close(sharedDataForEven[0]);
        close(sharedDataForEven[1]);
        close(sharedDataForOdd[0]);
        close(sharedDataForOdd[1]);
        err(1, "Error");

    }

    const char* debug = "Okay\n\0";
    write(1, debug, strlen(debug));

    close(sharedDataForEven[0]);
    close(sharedDataForEven[1]);
    close(sharedDataForOdd[0]);
    close(sharedDataForOdd[1]);
    exit(0);

}

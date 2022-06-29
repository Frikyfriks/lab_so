#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

void quit(int errNo)
{
    switch (errNo)
    {
    case 0:
        break;
    case 3:
        fprintf(stderr, "Incorrect input. Usage: ./NAME <target> <n>.\n");
        break;
    case 4:
        fprintf(stderr, "File already exists or directory given is invalid.\n");
        break;
    case 5:
        fprintf(stderr, "Incorrect input. <n> must be between 1 and 10.\n");
        break;
    case 6:
        fprintf(stderr, "Wfork failure.\n");
        break;
    case 7:
        fprintf(stderr, "Pipe reading error.\n");
        break;
     default:
        fprintf(stderr, "unknown (%d)", errNo);
        break;
    };
    }
}

int main(){
}
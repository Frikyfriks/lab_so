#include <unistd.h>   // fork, pipe
#include <stdlib.h>   // exit
#include <stdio.h>    // printf, fprintf, FILE, fopen, fclose
#include <stdbool.h>  // bool type, true=1, false=0
#include <signal.h>   // signals...

// check if a named file exists
bool file_exists(const char *target)
{
    FILE *fp = fopen(target, "r");
    bool exists = false;
    if (fp != NULL)
    {
        exists = true;
        fclose(fp);
    };
    return exists;
};
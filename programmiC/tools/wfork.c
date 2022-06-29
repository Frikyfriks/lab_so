// fork wrapper: fork, check result and return value
#include <unistd.h>   // fork, pipe
#include <stdlib.h>   // exit

int wfork()
{
    int f = fork();
    if (f < 0)
        quit(6);
    return f;
};

void main(){
    int figlio = wfork();
    if (figlio == 1){
        // Figlio exec
    }
    
}
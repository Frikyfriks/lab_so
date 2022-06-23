// Signal handler che non fa stoppare l'esecuzione
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signal){
    printf("CTRL+Z denied\n");
}

int main()
{
    // struttura dati per l'handler
    struct sigaction sa;
    sa.sa_handler = &handler;
    //Flag necessario per funzionare in questo caso
    sa.sa_flags = SA_RESTART;
    //Segnale da sovrascrivere, segnale da mandare, backup del segnale precedente
    sigaction(SIGTSTP, &sa, NULL);

    int x;
    printf("print a number\n");
    scanf (" %d", &x);
}
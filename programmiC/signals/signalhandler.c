// Signal handler che non fa stoppare l'esecuzione
/* Return di errori:
 * 1: Fork
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signal){
    printf("Some custome action\n");
}

int main()
{
    // struttura dati per l'handler
    struct sigaction sa;
    sa.sa_handler = &handler;
    //Flag necessario per funzionare in questo caso
    sa.sa_flags = SA_RESTART;
    //Segnale da sovrascrivere, segnale da mandare, backup del segnale precedente
    //SIGUSR1/2 Fatti a posta per l'utente
    sigaction(SIGUSR1, &sa, NULL);


    printf("Send Signal\n");
    kill(getpid(),SIGUSR1);
}
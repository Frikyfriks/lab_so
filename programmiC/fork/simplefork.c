// Processo crea un processo figlio
/*
 * Return di errori:
 * 1: Fork
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    //numero pid figlio, 0 = in exec, >0 = padre in exec, <0 = Errore
    int figlio;

    //numero per la gestione del wait
    int waitID;

    figlio = fork();
    // gestione d'errore fork
    if (figlio == -1)
    {
        perror("Errore nella fork\n");
        return 1;
    }

    // figlio execution
    if (figlio == 0)
    {
        //Mostra id padre e figlio
        printf("(figlio in exec) \n Padre pid: %d\n Figlio pid: %d\n\n",getppid(),getpid());
    }
    else
    { // padre execution
        //Mostra id padre e figlio
        printf("(padre in exec) \n Padre pid: %d\n Figlio pid: %d\n\n",getpid(),figlio);

        //Aspetta la fine dell'esecuzione del figlio per andare avanti
        wait(&waitID);
    }

    printf("Termine esecuzione processo: %d\n",getpid());
}
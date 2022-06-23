// Padre decide tramite SIGSTOP e SIGCONT quanto tempo il figlio può eseguire
/*
 * Return di errori:
 * 1: Fork
 * 2: Time inserito negativo
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    int figlio;

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
        while (1)
        {
            printf("Sto eseguendo\n");
        }
    }
    else
    { // padre execution

        int time;
        // Dobbiamo fermare il figlio altrimenti non vedremmo l'input
        kill(figlio, SIGSTOP);
        do
        {
            printf("\nImemtti quanto tempo può eseguire il processo figlio\n");
            scanf(" %d", &time);

            if (time < 0)
            {
                perror("Errore nell'input del tempo inserito\n");
                return 2;
            }

            // Avvio ed aspetto time per ribloccarlo
            kill(figlio, SIGCONT);
            sleep(time);
            kill(figlio, SIGSTOP);
        } while (time > 0);
    }
}
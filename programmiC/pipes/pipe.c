// Scrittura di un numero sul processo figlio e stampa dal processo padre
/*
 * Return di errori:
 * 1: Fork
 * 2: Apertura Pipe
 * 3: Scrittura Pipe
 * 4: Lettura Pipe
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    // 2 file descriptor per pipe, fd[0] => lettura, fd[1] => scrittura
    int file_descriptor[2];

    // apertura della pipe con gestione d'errore
    if (pipe(file_descriptor) == -1)
    {
        perror("Errore nell'apertura pipe\n");
        return 2;
    }

    // Shared variables
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
        // figlio scrive quindi chiudiamo la lettura
        close(file_descriptor[0]);

        // input figlio
        int n;
        printf("Immetti n\n");
        scanf(" %d", &n);

        // scrive input sulla pipe con gestione d'errore
        if (write(file_descriptor[1], &n, sizeof(int)) == -1)
        {
            perror("Errore nella scrittura figlio\n");
            return 3;
        }

        // fine utilizzo pipe, chiusura canale
        close(file_descriptor[1]);
    }
    else
    { // padre execution
        // padre legge quindi chiudiamo la scrittura
        close(file_descriptor[1]);

        int x;
        // legge input figlio sulla pipe con gestione d'errore
        if (read(file_descriptor[0], &x, sizeof(int)) == -1)
        {
            perror("Errore nella lettura padre\n");
            return 4;
        }
        printf("Padre stampa, numero inserito e': %d \n", x);
    }
}
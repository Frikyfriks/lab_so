// Scrittura di un numero su entrambi e stampa di operazioni
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
    // Due file descriptor per pipe
    int p_f[2], f_p[2];

    // apertura delle pipe con gestione d'errore
    if (pipe(p_f) == -1)
    {
        perror("Errore nell'apertura pipe 1\n");
        return 2;
    }
    if (pipe(f_p) == -1)
    {
        perror("Errore nell'apertura pipe 2\n");
        return 2;
    }

    // Shared variables
    int figlio, n, result;

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
        // figlio legge da f_p e scrive da p_f, chiudiamo i canali non usati
        close(p_f[0]);
        close(f_p[1]);

        // input figlio
        printf("Immetti n (figlio)\n");
        scanf(" %d", &n);

        // scrive input sulla pipe con gestione d'errore
        if (write(p_f[1], &n, sizeof(int)) == -1)
        {
            perror("Errore nella scrittura figlio\n");
            return 3;
        }

        // legge input padre sulla pipe con gestione d'errore
        if (read(f_p[0], &result, sizeof(int)) == -1)
        {
            perror("Errore nella lettura figlio\n");
            return 4;
        }

        // Chiusura comunicazioni dopo l'utilizzo
        close(p_f[1]);
        close(f_p[0]);

        // codice figlio
        result = result + n;
        printf("Il risultato del figlio, ossia la somma e': %d \n", result);
    }
    else
    { // padre execution
        // padre legge da p_f e scrive da f_p, chiudiamo i canali non usati
        close(p_f[1]);
        close(f_p[0]);

        // input padre, stoppiamo il figlio per non incasinare gli input
        kill(figlio, SIGSTOP);
        printf("Immetti n (padre)\n");
        scanf(" %d", &n);
        kill(figlio, SIGCONT);

        // scrive input sulla pipe con gestione d'errore
        if (write(f_p[1], &n, sizeof(int)) == -1)
        {
            perror("Errore nella scrittura padre\n");
            return 3;
        }

        // legge input figlio sulla pipe con gestione d'errore
        if (read(p_f[0], &result, sizeof(int)) == -1)
        {
            perror("Errore nella lettura padre\n");
            return 4;
        }

        // Chiusura comunicazioni dopo l'utilizzo
        close(p_f[0]);
        close(f_p[1]);

        // codice padre
        result = result * n;
        printf("Il risultato del padre, ossia la moltiplicazione e': %d \n", result);
    }
}
// Avendo come argomenti dei “binari”, si eseguono con exec ciascuno in un sottoprocesso
// ma in più salvando i flussi di stdout e stderr in un unico file
/*
 * Return di errori:
 * 1: Fork
 * 2: Exec programma
 */
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{

    // Questo sara' il pid-1 di tutti gli altri binari, exec cambia proprio il processo in esecuzione
    printf("[BINARI.C]\n processo numero:%i \n", getpid());
    int i = 1;
    int figlio, waitID;
    int fd = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd,1);
    dup2(fd,2);
    while (i < argc)
    {
        figlio = fork();
        if (figlio < 0)
        {
            perror("Errore nella creazione del figlio \n");
            return 1;
        }
        if (figlio > 0)
        {
            printf("Sono il padre che stampa, perché fork ha ritornato il valore %i\n", figlio);
            wait(&waitID);
        }
        else
        {
            char *command[] = {argv[i], NULL};
            // File binario da eseguire
            char *bin_file = command[0];
            if (execve(bin_file, command, NULL) == -1)
            {
                printf("Errore nell'esecuzione %s\n", bin_file);
                return 2;
            }
        }

        i++;
    }
}

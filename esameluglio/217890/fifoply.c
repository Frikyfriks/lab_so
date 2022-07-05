#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void quit(int code)
{
    switch (code)
    {
    case 1:
        // Non legge la FIFO
        break;
    case 2:
        // Argomenti sbagliati
        break;
    default:
        printf("Errore non gestito");
        break;
    }
    exit(code);
}

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        quit(2);
    }

    //Creazione FIFO
    mkfifo(argv[1], 0777);

    int file_descriptor = open(argv[1], O_RDONLY);
    if (file_descriptor < 0)
    {
        quit(2);
    }

    int i;
    char dato;
    int n = atoi(argv[2]);
    char numeri [10];
    char caratteri[10];
    int i_numeri, i_char;

    for (i = 0; i < 10; i++)
    {
        if (read(file_descriptor, &dato, sizeof(char)) == -1)
        {
            // Finiti input
            i = 10;
        }
        else if(&dato >= "A" && &dato <= "Z"){
            caratteri[i_char] = dato;
            i_char = i_char + 1;
        }
        else if (i_numeri == n)
        {
            i = 10;
        }
        else {
            caratteri[i_numeri] = dato;
            i_numeri = i_numeri + 1;
        }
    }

    for (;i_char < 0; i_char--){
        printf("%c\n",caratteri[i_char-1]);
    }

    int somma;
    for (;i_numeri < 0; i_numeri--){
        if (i_numeri = 0)
            printf("%c=\n",numeri[i_numeri]);
        else
            printf("%c+\n",numeri[i_numeri]);
        somma = somma + atoi(&numeri[i_numeri]);
    }
    printf("%d",somma);

    return(0);
}
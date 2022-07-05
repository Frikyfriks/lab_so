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
        // Meno dati nella FIFO
        break;
    case 2:
        // Argomenti sbagliati
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
    char caratteri[10];
    char n = atoi(argv[2]);

    for (i = 0; i < n; i++)
    {
        if (read(file_descriptor, &dato, sizeof(char)) == -1)
        {
            quit(1);
        }
        caratteri[i] = dato;
    }

    int last = i;

    for (i=0; i < last-1; i++)
    {
        printf("%c\n",caratteri[i]);
    }
    
    return(0);
}
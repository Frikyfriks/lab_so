/*
Applicazione che data una stringa come argomento
ne stampa a video la lunghezza
*/

#include <stdio.h>
#include <string.h>

void main(int argc, char **argv)
{

    long int sum;
    int i;

    for (i = argc-1; i > 0; i--)
    {
        sum = sum + strlen(argv[i]);
    }
    printf(" %ld\n",sum);
}
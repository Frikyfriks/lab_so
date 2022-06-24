//Il sorting non funziona ma aiuta per il passaggio di argomenti dei thread
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// Routine che esegue il thread
void *sorting_routine(int *array)
{
    int i = array[0];
    int m = array[1];

    if (array[i] < array[m])
    {
        int temp = array[i];
        array[i] = array[m];
    }
}

// funzione gestione errori
void quit(int err)
{
    printf("Errore\n");
}

int main()
{
    // Thread structure
    pthread_t t1, t2;

    int array[6];
    printf("Immetti i 4 numeri\n");
    for (int i = 2; i < 6; i++)
    {
        scanf(" %d", &array[i]);
    }

    array[0] = 2;
    array[1] = 3;
    // Creazione thread con gestione d'errore
    if (pthread_create(&t1, NULL, sorting_routine, (int *)&array) == -1)
    {
        quit(1);
    }

    array[0] = 4;
    array[1] = 5;
    if (pthread_create(&t2, NULL, sorting_routine, (int *)&array) == -1)
    {
        quit(1);
    }

    // Wait della thread
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Cancellazione della thread
    pthread_cancel(t1);
    pthread_cancel(t2);

    if (array[2] < array[4])
    {
        int temp = array[2];
        array[2] = array[4];
        array[4] = temp;
        temp = array[3];
        array[3] = array[5];
        array[5] = temp;
    }

    printf("Array sorted: ");
    for (int i = 2; i <6; i++){
        printf(" %d",array[i]);
    }
    printf("\n");
    return 0;
}
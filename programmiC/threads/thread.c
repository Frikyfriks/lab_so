#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//Routine che esegue il thread
void* routine(){
    printf("Executing thread\n");
}

int main (){
    //Thread structure
    pthread_t t1;

    //Creazione thread
    pthread_create(&t1,NULL,&routine,NULL);

    //Wait della thread
    pthread_join(t1,NULL);

    //Cancellazione della thread
    pthread_cancel(t1);

    return 0;
}
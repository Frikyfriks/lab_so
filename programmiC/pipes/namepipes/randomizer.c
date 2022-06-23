//Randomizza ogni volta un numero e lo scrive nel FIFO file
#include <stdio.h>
#include <unistd.h>
#include <time.h>

//Librerie specifiche
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (){

    //Creazione FIFO file 0777 sono i permessi, come chmod
    if (mkfifo("Randnumber",0777)== -1){
        printf("Errore nella creazione del FifoFile, forse è già esistente\n");
    }

    //apertura Fifo File in write only
    int file_descriptor = open("Randnumber", O_WRONLY);
    srand(time(NULL));
    int i;
    int numero;
    for (i=0; i<4; i++){
        numero = rand()%100;
        // Non va avanti fino a che qualcuno non legge il numero
        if (write(file_descriptor,&numero,sizeof(int)) == -1){
            perror("Errore nella scrittura del FifoFile");
        }
    }
    close (file_descriptor);
    printf("Done\n");
}


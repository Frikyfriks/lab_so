#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <signal.h>

int son_has_done(int sig)
{
}

// struttura buffer sia del send che del receive
struct msg_buffer
{
    long mtype;
    int num;
} msgpSND, msgpRCV;

int main()
{
    int waitID;
    srand(time(NULL));

    // Dichiara struttura per la modifica
    struct msqid_ds mod;

    // Restituisce una chiave basandosi sul path
    key_t queue1Key = ftok("/tmp/unique", 1);

    // Creazione queue
    int queueID = msgget(queue1Key, 0777 | IPC_CREAT);

    int figlio = fork();
    if (figlio == -1)
    {
        perror("Errore Fork\n");
        return 1;
    }

    if (figlio == 0)
    {
        // Figlio execution

        int i = 0;
        msgpSND.mtype = 1;
        while (i < 10)
        {
            msgpSND.num = rand() % 10;
            // Manda messaggio
            msgsnd(queueID, &msgpSND, sizeof(msgpSND.num), 0);
            printf("Numero mandato: %d\n", msgpSND.num);
            i++;
            //Solo per far vedere come funziona a velocità diverse
            sleep(1);
        }
    }
    else
    {
        // Padre execution
        int i = 0;
        while (i < 10)
        {
            // Ricezione messaggio
            msgrcv(queueID, &msgpRCV, sizeof(msgpRCV.num), 1, 0);
            printf("Numero ricevuto: %d\n", msgpRCV.num);
            //Solo per far vedere come funziona a velocità diverse
            sleep(2);
            i++;
        }
        wait(&waitID);
    }
    printf("%d Done\n", getpid());
}
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
// struttura buffer sia del send che del receive
struct msg_buffer
{
    long mtype;
    char mtext[100];
} msgpSND, msgpRCV;

void main()
{
    struct msqid_ds mod;

    msgpSND.mtype = 1;

    // testo send = This is a message from sender
    strcpy(msgpSND.mtext, "This is a message from sender");

    // Restituisce una chiave basandosi sul path
    key_t queue1Key = ftok("/tmp/unique", 1);

    #pragma region //Example istruction set
    /* [Crea la coda]
     * int queueId = msgget(queue1Key, 0777 | IPC_CREAT);
     * [Rimuove la code se esiste]
     * msgctl(queueId, IPC_RMID, NULL);
     * [Mandare messaggi]
     * msgsnd(queueId, &msgpSND, sizeof(msgpSND.mtext), 0);
     * [Ricevere messaggi]
     * msgrcv(queueId, &msgpRCV, sizeof(msgpRCV.mtext), 1, 0)
     */
    #pragma endregion

    int queueId;
    queueId = msgget(queue1Key, 0777 | IPC_CREAT);       // Create queue
    msgsnd(queueId, &msgpSND, sizeof(msgpSND.mtext), 0); // Send msg
    msgsnd(queueId, &msgpSND, sizeof(msgpSND.mtext), 0); // Send msg
    msgsnd(queueId, &msgpSND, sizeof(msgpSND.mtext), 0); // Send msg
    msgctl(queueId, IPC_STAT, &mod);                     // Modify queue

    printf("Msg in queue: %ld\nCurrent max bytes in queue: %ld\n\n", mod.msg_qnum, mod.msg_qbytes);
    mod.msg_qbytes = 200;           // Change buf to modify queue bytes
    msgctl(queueId, IPC_SET, &mod); // Apply modification

    printf("Msg in queue: %ld --> same number\nCurrent max bytes in queue: %ld\n\n", mod.msg_qnum, mod.msg_qbytes);
    if (fork() != 0)
    { // Parent keep on writing on the queue
        printf("[SND] Sending 4th message with a full queue...\n");
        msgsnd(queueId, &msgpSND, sizeof(msgpSND.mtext), 0); // Send msg
        printf("[SND] msg sent\n");
        printf("[SND] Sending 5th message with IPC_NOWAIT\n");
        if (msgsnd(queueId, &msgpSND, sizeof(msgpSND.mtext), IPC_NOWAIT) == -1)
        { // Send msg
            perror("Queue is full --> Error");
        }
    }
    else
    { // Child keeps reading the queue every 3 seconds
        sleep(3);
        msgrcv(queueId, &msgpRCV, sizeof(msgpRCV.mtext), 1, 0);
        printf("[Reader] Received msg 1 with msg '%s'\n", msgpRCV.mtext);
        sleep(3);
        msgrcv(queueId, &msgpRCV, sizeof(msgpRCV.mtext), 1, 0);
        printf("[Reader] Received msg 2 with msg '%s'\n", msgpRCV.mtext);
        sleep(3);
        msgrcv(queueId, &msgpRCV, sizeof(msgpRCV.mtext), 1, 0);
        printf("[Reader] Received msg 3 with msg '%s'\n", msgpRCV.mtext);
        sleep(3);
        msgrcv(queueId, &msgpRCV, sizeof(msgpRCV.mtext), 1, 0);
        printf("[Reader] Received msg 4 with msg '%s'\n", msgpRCV.mtext);
        sleep(3);
        if (msgrcv(queueId, &msgpRCV, sizeof(msgpRCV.mtext), 1, IPC_NOWAIT) == -1)
        {
            perror("Queue is empty --> Error");
        }
        else
        {
            printf("[Reader] Received msg 5 with msg '%s'\n",
                   msgpRCV.mtext);
        }
    }
    while (wait(NULL) > 0)
        ;
}
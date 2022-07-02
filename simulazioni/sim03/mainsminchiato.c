#define DEBUG 1

#define PIPE_READ 0
#define PIPE_WRITE 1
#define PID_LEN 8
#define MAX_CHILDREN 10
#define MAX_CHAR 32

#include <unistd.h>    // fork, pipe
#include <stdlib.h>    // exit
#include <stdio.h>     // printf, fprintf, FILE, fopen, fclose
#include <stdbool.h>   // bool type, true=1, false=0
#include <signal.h>    // signals...
#include <string.h>    // strlen
#include <sys/wait.h>  // wait
#include <errno.h>     // errno, strerror
#include <sys/types.h> // queue types
#include <sys/ipc.h>   // queue IPC
#include <sys/msg.h>   // queue msg

FILE *fp;                 // handler for log file stated in argument <path>
char queuePath[MAX_CHAR]; // <name> passato come argomento
char action[MAX_CHAR];    // <action> passato come argomento
char pid[MAX_CHAR];       // <pid> passato come argomento

void quit(int code)
{
    int pid = getpid();
    if (code > 0)
        fprintf(stderr, "?Error (process %d): ", pid);
    switch (code)
    {
    case 0:;
        break;
    // ------------------------------
    case 1:
        fprintf(stderr, "wrong usage");
        break;
    case 2:
        fprintf(stderr, "wrong <name>");
        break;
    case 3:
        fprintf(stderr, "wrong <action>");
        break;
    case 4:
        fprintf(stderr, "wrong <value>");
        break;
    case 5:
        fprintf(stderr, "wrong <pid>");
        break;
    // ------------------------------
    case 6:
        fprintf(stderr, "fork failure");
        break;
    case 7:
        fprintf(stderr, "sprintf failure");
        break;
    case 8:
        fprintf(stderr, "pipe write failure");
        break;
    case 9:
        fprintf(stderr, "pipe read failure");
        break;
    case 10:
        fprintf(stderr, "pipe creation failure");
        break;
    // ------------------------------
    default:
        fprintf(stderr, "unknown (%d)", code);
        break;
    };

    if (code > 0)
        fprintf(stderr, ".\n");
    fflush(stderr);
    if (DEBUG == 1)
        fprintf(stderr, "quitting process %d\n",
                pid);
    exit(code);
};

// check if a named file exists
bool file_exists(const char *target)
{
    FILE *fp = fopen(target, "r");
    bool exists = false;
    if (fp != NULL)
    {
        exists = true;
        fclose(fp);
    };
    return exists;
};

// struttura buffer della queue
struct msg_buffer
{
    long mtype;
    char value[MAX_CHAR]; // <value> passato come argomento
} msgpSND, msgpRCV;

key_t createQueue()
{
    // Restituisce una chiave basandosi sul path
    key_t queue1Key = ftok(queuePath, 1);

    // Creazione queue
    int queueID = msgget(queue1Key, 0777 | IPC_CREAT);

    return queue1Key;
}

int retrieveQueueID(queueKey)
{
    // Recupero queueID
    int queueID = msgget(queueKey, 1);
    return queueID;
}

void actionHandler(char *action)
{
    struct msqid_ds mod;
    // Restituisce una chiave basandosi sul path
    key_t queue1Key = ftok("/tmp/unique", 1);

    // Creazione queue
    int queueID = msgget(queue1Key, 0777 | IPC_CREAT);
    msgpSND.mtype = 1;

    // NON ESISTE SWITCH PER STRINGHE C MERDA
    if (strcmp("new", action) == 0)
    {
        printf("queueID: %d\n", queueID);
    }
    else if (strcmp("put", action) == 0)
    {
        msgsnd(queueID, &msgpSND, sizeof(msgpSND.value), 0);
    }
    else if (strcmp("get", action) == 0)
    {

        msgrcv(queueID, &msgpRCV, sizeof(msgpRCV.value), 1, 0);
        printf("Messaggio recuperato %s\n", &msgpRCV.value);
    }
    else if (strcmp("del", action) == 0)
    {
        msgctl(queueID, IPC_RMID, NULL);
    }
    else if (strcmp("emp", action) == 0)
    {
        char getted[MAX_CHAR];
        while (msgrcv(queueID, &msgpRCV, sizeof(msgpRCV.value), 1, 0) != -1)
        {
            printf("Messaggio recuperato %s\n", msgpRCV.value);
        }
    }
    else
        quit(3);
}

int main(int argc, char **argv)
{

    if (argc != 4 && argc != 5)
        quit(1);

    strcpy(queuePath, argv[1]); // queuePath = argv[1];
    strcpy(action, argv[2]);    // action = argv[2]
    if (argc == 5)
    {
        strcpy(msgpSND.value, argv[3]); // value = argv[3];
        strcpy(pid, argv[4]);           // pid = argv[4];
    }
    else
        strcpy(pid, argv[3]); // pid = argv[3];

    if (DEBUG == 1)
    {
        printf("queuePath: %s\n", queuePath);
        printf("action: %s\n", action);
        printf("value: %s\n", msgpSND.value);
        printf("pid: %s\n", pid);
    }

    actionHandler(action);
    return 0;
}
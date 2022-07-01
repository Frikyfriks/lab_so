#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define PIPE_READ 0
#define PIPE_WRITE 1
#define PID_LEN 8
#define MAX_CHILDREN 10

int channel[MAX_CHILDREN][2];  //Pipe per comunicazione
int nfork = 0;                 //Numero di figli attivi
FILE *fp;                      //File di log

//Struttura per l'input da tastiera
typedef enum
{
    KP_ECHO_OFF,
    KP_ECHO_ON,
} kp_echo_t;

//Handler input tasto premuto
int keypress(const kp_echo_t echo)
{
    struct termios savedState, newState;
    unsigned char echo_bit; // flag
    int c;
    if (-1 == tcgetattr(STDIN_FILENO, &savedState))
    {
        return EOF;
    }; // error
    newState = savedState;
    if (KP_ECHO_OFF == echo)
    {
        echo_bit = ECHO;
    }
    else
    {
        echo_bit = 0;
    };
    /* canonical input + set echo with minimal input as 1. */
    newState.c_lflag &= ~(echo_bit | ICANON);
    newState.c_cc[VMIN] = 1;
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
    {
        return EOF;
    };             // error
    c = getchar(); /* block until key press */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
    {
        return EOF;
    }; // error
    return c;
}

//Funzione esistenza file
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

//handler dei vari exit con relativa stampa
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
    case 3:
        fprintf(stderr, "wrong usage");
        break;
    case 4:
        fprintf(stderr, "wrong first argument, it can be server or client");
        break;
    case 5:
        fprintf(stderr, "wrong <file> or already exist");
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

//wrapper fork con gestione d'errore
int wfork()
{
    int f = fork();
    if (f < 0)
        quit(6);

    if (write(channel[nfork][PIPE_WRITE], getpid(), sizeof(int)))
        quit(8);

    nfork = nfork + 1;
    return f;
};

//handler manager per il segnale ricevuto dal server
void handlerManager(int signo, siginfo_t *info, void *empty)
{
    int sender;
    sender = (int)info->si_pid; // retrieve pid of sender

    if pipe (channel[nfork] < 0)
        quit(10);

    switch (signo)
    {
    case SIGUSR1:
        int figlio = wfork();
        if (figlio > 0)
        {
            printf("[server] +%d", figlio);
            fprintf(fp, "+%d", figlio);
            fflush(fp);
        }
        break;

    case SIGUSR2:
        if (nfork == 0)
        {
            fprintf(fp, "0");
            fflush(fp);
            printf("[server] 0\n")
        }
        else
        {
            int pid_to_kill;
            read(channel[nfork - 1][PIPE_READ], &pid_to_kill, sizeof(int));
            fprintf(fp, "-%d", pid_to_kill);
            fflush(fp);
            printf("[server] -%d", pid_to_kill);
            kill(pid_to_kill, SIGKILL);
        }
        break;

    case SIGINT:
        fprintf(fp, "ancora presenti %d figli", nfork);
        fflush(fp);
        quit(0);
        break;
    }
};

//Esecuzione server
void serverfunc()
{

    fprintf(fp, "%d\n", getpid());
    fflush(fp);
    printf("[server:%d", getpid());

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_SIGINFO;
    sa.sa_flags |= SA_RESTART;
    sa.sa_sigaction = handlerManager;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
}

//Esecuzione client
void clientfunc()
{
    /*
     * After initialization read keyboard and send command to "server"
     * using a signal
     */
    char c;
    int ppid;
    fp = 0;

    // Initialize logfile (try to read pid from first line)
    while (!(fp > 0))
    {
        fp = fopen(logfile, "r");
        sleep(1);
    };
    fscanf(fp, "%d", &ppid);
    fclose(fp);
    printf("[client] server: %d\n", ppid);
    // idle:
    while (1)
    {
        c = keypress(KP_ECHO_OFF); // read single keypress without echoing
        if (c == '+')
        { // ask to add a new child
            if (nfork < MAX_CHILDREN)
            {
                nfork++;
                k = kill(ppid, SIGUSR1);
                {
                    fprintf(stderr, "\n?[server] error sending SIGUSR1 to server.\n\n");
                    exit(6);
                };
                printf("[client] %d\n", nfork);
            };
        };
        if (c == '-')
        { // ask to remove a child
            if (nfork > 0)
            {
                nfork--;
                k = kill(ppid, SIGUSR2);
                if (k < 0)
                {
                    fprintf(stderr, "\n?[server] error sending SIGUSR2 to server.\n\n");
                    exit(6);
                };
                printf("[client] %d\n", nfork);
            };
        };
        if (c == '\n')
        { // ask to remove all children then quit
            while (nfork > 0)
            {
                nfork--;
                k = kill(ppid, SIGUSR2);
                if (k < 0)
                {
                    fprintf(stderr, "\n?[server] error sending SIGUSR2 to server.\n\n");
                    exit(6);
                };
                printf("[client] %d\n", nfork);
                sleep(1);
            };
            k = kill(ppid, SIGINT);
            if (k < 0)
            {
                fprintf(stderr, "\n?[server] error sending SIGINT to server.\n\n");
                exit(6);
            };
            exit(0);
        };
    };
    exit(0);
}

int main(int argc, char **argv)
{

    // CHECK ARGUMENTS:
    if (argc != 3)
    {
        quit(3);
    }

    argv[1] = tolower(argv[1]);

    if (strcmp(argv[1], "server"))
    {
        if (file_exists(file[1]))
            quit(4);

        fp = fopen(file[1], O_WRONLY);
        if (fp == NULL)
            quit(4);
        serverfunc();
    }
    else if (strcmp(argv[1], "client"))
    {
        int serverpid;
        while (!(fp > 0))
        {
            fp = fopen(file[1], O_RDONLY);
            sleep(1)
        }

        clientfunc();
    }
    else
    {
        quit(4);
    }
}

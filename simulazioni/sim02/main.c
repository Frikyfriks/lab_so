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

int channel[MAX_CHILDREN][2]; // Pipe per comunicazione
int nfork = 0;                // Numero di figli attivi
FILE *fp;                     // File di log

// Struttura per l'input da tastiera
typedef enum
{
    KP_ECHO_OFF,
    KP_ECHO_ON,
} kp_echo_t;

// Handler input tasto premuto
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

// Funzione esistenza file
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

// handler dei vari exit con relativa stampa
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
    case 11:
        fprintf(stderr, "\n?[server] error sending SIGUSR1 to server.\n\n");
        break;
    case 12:
        fprintf(stderr, "\n?[server] error sending SIGUSR1 to server.\n\n");
        break;
    case 13:
        fprintf(stderr, "\n?[server] error sending SIGINT to server.\n\n");
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

// wrapper fork con gestione d'errore
int wfork()
{
    int f = fork();
    if (f < 0)  //Gestione errore fork
        quit(6);

    if (write(channel[nfork][PIPE_WRITE], getpid(), sizeof(int)) < 0) //Passaggio del pid del figlio sulla pipe con gestione in caso d'errore
        quit(8);

    nfork = nfork + 1;
    return f;
};

// handler manager per il segnale ricevuto dal server
void handlerManager(int signo, siginfo_t *info, void *empty)
{
    int sender;
    sender = (int)info->si_pid; // recupera pid del sender

    if pipe (channel[nfork] < 0) // Creazione della pipe con gestione dell'errore
        quit(10);

    //handling diverso in base al signal
    switch (signo)
    {
    case SIGUSR1: // [4 Punti] genera figlio e stampa in log e stdout
        int figlio = wfork();
        if (figlio > 0)
        {
            printf("[server] +%d", figlio);
            fprintf(fp, "+%d", figlio);
            fflush(fp);
        }
        break;

    case SIGUSR2: // [4 Punti] termina figlio o stampa 0 se non esistono
        if (nfork == 0)
        {
            fprintf(fp, "0");
            fflush(fp);
            printf("[server] 0\n")
        }
        else
        {
            int pid_to_kill; //pid figlio da terminare
            // leggi dalla pipe il pid del figlio da terminare
            read(channel[nfork - 1][PIPE_READ], &pid_to_kill, sizeof(int));
            fprintf(fp, "-%d", pid_to_kill);
            fflush(fp);
            printf("[server] -%d", pid_to_kill);
            kill(pid_to_kill, SIGKILL);
        }
        break;

    case SIGINT: // Stampa figli presenti e termina
        fprintf(fp, "ancora presenti %d figli", nfork);
        fflush(fp);
        int k, pid_to_kill;
        //kill di tutti i figlio per non lasciarli orfani
        for (k = 0; k < nfork; k++)
        {
            read(channel[nfork - 1][PIPE_READ], &pid_to_kill, sizeof(int));
            kill(children[k], SIGKILL);
        };
        fclose(fp);
        quit(0);
        break;
    }
};

// Esecuzione server
void serverfunc()
{
    // [5 Punti] Stampa il pid sul file di log e in stdout e gestisci i segnali
    fprintf(fp, "%d\n", getpid());
    fflush(fp);
    printf("[server:%d", getpid());

    //Strutttura gestione segnali
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_SIGINFO;
    sa.sa_flags |= SA_RESTART;
    sa.sa_sigaction = handlerManager;
    //Segnali da gestire
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
}

// Esecuzione client
void clientfunc()
{
    /*
     * After initialization read keyboard and send command to "server"
     * using a signal
     */

    // [5 Punti] leggi dal log ed aspetta una pressione di un tasto

    char c;        // Tasto premuto
    int serverpid; // Process id server

    fscanf(fp, "%d", &serverpid);
    fclose(fp);
    printf("[client] server: %d\n", serverpid);
    // idle:
    while (1)
    {
        c = keypress(KP_ECHO_OFF); // read single keypress without echoing

        // [4 Punti] richiesta al server di aumentare o diminuire figli con gestione del massimo
        if (c == '+')
        { // ask to add a new child
            if (nfork < MAX_CHILDREN)
            {
                nfork++;
                k = kill(serverpid, SIGUSR1);
                {
                    quit(11);
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
                k = kill(serverpid, SIGUSR2);
                if (k < 0)
                {
                    quit(12);
                    exit(6);
                };
                printf("[client] %d\n", nfork);
            };
        };

        //[4 Punti] alla pressione di Enter SIGUSR2 tante volte quanti figli
        if (c == '\n')
        { // ask to remove all children then quit
            int k;
            while (nfork > 0)
            {
                nfork--;
                k = kill(serverpid, SIGUSR2);
                if (k < 0)
                {
                    quit(12);
                    exit(6);
                };
                printf("[client] %d\n", nfork);
                sleep(1);
            };
            k = kill(serverpid, SIGINT);
            if (k < 0)
            {
                quit(13);
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

    //Evita ambiguità Client con client, cliEnt ecc...
    argv[1] = tolower(argv[1]);

    if (strcmp(argv[1], "server"))
    {
        if (file_exists(file[1]))
            quit(4);

        fp = fopen(argv[1], O_WRONLY);
        if (fp == NULL)
            quit(4);
        serverfunc();
    }
    else if (strcmp(argv[1], "client"))
    {
        int serverpid;
        while (!(fp > 0))
        {
            fp = fopen(argv[1], O_RDONLY);
            sleep(1)
        }
        clientfunc();
    }
    else
    {
        quit(4);
    }
}

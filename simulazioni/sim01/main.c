/*
Esempio "main.c"
*/
#define DEBUG 0

#define PIPE_READ 0
#define PIPE_WRITE 1
#define PID_LEN 8
#define MAX_CHILDREN 10

#include <unistd.h>   // fork, pipe
#include <stdlib.h>   // exit
#include <stdio.h>    // printf, fprintf, FILE, fopen, fclose
#include <stdbool.h>  // bool type, true=1, false=0
#include <signal.h>   // signals...
#include <string.h>   // strlen
#include <sys/wait.h> // wait
#include <errno.h>    // errno, strerror
FILE *fp;             // handler for log file stated in argument <path>

int n;                                  // storage for argument <n>
int pid_main;                           // pid of main process
int pid_manager;                        // pid of "manager"
int pid_leaf, pid_leaves[MAX_CHILDREN]; // pid of leaf (each) + list of leaves(in "manager") int leaf_counter = 0;                                                                                                                                                                                                                                                                               // first available leaf (in"manager")
int channel[MAX_CHILDREN][2];           // pipes between "manager" and "leaves"

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

// quit, possibly printing a message on stderr, with stated exit code
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
        fprintf(stderr, "wrong <target>");
        break;
    case 5:
        fprintf(stderr, "wrong <n>");
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

// fork wrapper: fork, check result and return value
int wfork()
{
    int f = fork();
    if (f < 0)
        quit(6);
    return f;
};

// LEAF PROCESS
void funcLeaf(int l)
{
    char spid[PID_LEN];
    // int l : index of leaf
    pid_leaf = getpid();
    // DUMP PID TO LOG:
    fprintf(fp, "%d\n", pid_leaf);
    fflush(fp);
    // WAIT FOR TRIGGER TO ANSWER SIGNALING:
    close(channel[l][PIPE_WRITE]);
    if (read(channel[l][PIPE_READ], spid, PID_LEN) < 0)
        quit(9);
    kill(atoi(spid), SIGUSR2);
    quit(0);
};

// MANAGER PROCESS
// look for pid inside list of leaves to retrieve its index
int leafIndex(int pid)
{
    int i = -1, l;
    for (l = 0; l < MAX_CHILDREN; l++)
    {
        if (pid_leaves[l] == pid)
        {
            i = l;
            break;
        };
    };
    return i;
};
void handlerManager(int signo, siginfo_t *info, void *empty)
{
    char spid[PID_LEN];
    int sender = info->si_pid;
    int leaf_counter;
    if (sprintf(spid, "%d", sender) < 0)
        quit(7);
    if (DEBUG == 1)
        printf("Manager (%d) writing to pipe #%d '%s'\n", getpid(), leaf_counter, spid);
    if (write(channel[leaf_counter++][PIPE_WRITE], spid,
              strlen(spid) + 1) < 0)
        quit(8);
    if (leaf_counter == MAX_CHILDREN)
        quit(0);
};
void funcManager()
{
    pid_manager = getpid();
    fprintf(fp, "%d\n", pid_manager);
    fflush(fp);
    int l;
    for (l = 0; l < n; l++)
    {
        if (pipe(channel[l]) < 0)
            quit(10);
        pid_leaf = wfork();
        if (pid_leaf == 0)
            funcLeaf(l);
        if (DEBUG == 1)
            printf("Manager (%d), closing reading-side of pipe #%d\n", getpid(), l);
        close(channel[l][PIPE_READ]);
        pid_leaves[l] = pid_leaf;
    };

    // SET SIGNAL HANDLER:
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_SIGINFO;
    sa.sa_flags |= SA_RESTART;
    sa.sa_sigaction = handlerManager;
    sigaction(SIGUSR1, &sa, NULL);
    // WAIT FOR ALL CHILDREN, so exit if all exit
    while (wait(NULL) > 0)
        ;
    quit(0);
};

// MAIN PROCESS
int main(int argc, char **argv)
{
    if (DEBUG == 1)
        printf("\nMAIN (%d)\n", getpid());
    // CHECK ARGUMENTS:
    if (argc != 3)
    {
        quit(3);
    };
    n = atoi(argv[2]);
    if (n < 1 || n > 10)
        quit(5);
    if (file_exists(argv[1]))
        quit(4);
    
    // MAIN
    fp = fopen(argv[1], "w");
    if (fp == NULL)
        quit(4);
    pid_main = getpid();
    fprintf(fp, "%d\n", pid_main);
    fflush(fp);
    pid_manager = wfork();
    if (pid_manager == 0)
        funcManager();
    fclose(fp);
    return 0;
};
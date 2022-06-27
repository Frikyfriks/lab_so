/* Dati due eseguibili come argomenti del tipo ls e wc si eseguono in due processi distinti: il primo
 * deve generare uno stdout redirezionato su un file temporaneo, mentre il secondo deve essere
 * lanciato solo quando il primo ha finito leggendo lo stesso file come stdin.
 * Ad esempio ./main ls wc deve avere lo stesso effetto di ls | wc.
 */
#define MAXLEN 100
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
    int a = 0, num = argc - 1, f;
    char cmd[MAXLEN], *lst[2];
    int passfile;
    if (num != 2)
    {
        printf("?Error. Usage: ... <cmd1> <cmd2>, to emulate <cmd1> | <cmd2>\n");
        exit(2);
    };
    printf("[%d] running '%s'...\n", getpid(), argv[1]);
    f = fork();
    if (f == 0)
    {
        passfile = open("/tmp/pass.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (passfile < 0)
        {
            printf("?Error. Cannot open output file\n");
            exit(3);
        };
        if (dup2(passfile, 1) < 0)
        {
            printf("?Error. Cannot dup2 output\n");
            exit(4);
        };
        close(passfile); // copy passfile to FD 1
        strcpy(cmd, argv[1]);
        lst[0] = cmd;
        lst[1] = NULL;
        execvp(cmd, lst);
    };
    while (wait(NULL) > 0)
        ;
    printf("[%d] child %d ok\n", getpid(), f);
    sleep(2);
    printf("[%d] running '%s'...\n", getpid(), argv[2]);
    f = fork();
    if (f == 0)
    {
        passfile = open("/tmp/pass.txt", O_RDONLY, S_IRUSR | S_IWUSR);
        if (passfile < 0)
        {
            printf("?Error. Cannot open input file\n");
            exit(3);
        };
        if (dup2(passfile, 0) < 0)
        {
            printf("?Error. Cannot dup2 input\n");
            exit(5);
        };
        close(passfile); // copy passfile to FD 0
        strcpy(cmd, argv[2]);
        lst[0] = cmd;
        lst[1] = NULL;
        execvp(cmd, lst);
    };
    while (wait(NULL) > 0)
        ;
    printf("[%d] child %d ok\n", getpid(), f);
    sleep(2);
    printf("[%d] done.\n", getpid());
}
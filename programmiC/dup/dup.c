#include <unistd.h> //dup.c
#include <stdio.h>
#include <fcntl.h>
void main()
{
    char buf[50];
    int fd = open("file.txt", O_RDWR); // file exists
    read(fd, buf, 50);
    printf("Content: %s\n", buf);
    int cpy = dup(fd);                // Create copy of file descriptor
    dup2(cpy, 22);                    // Copy cpy to descriptor 22 (close 22 if opened)
    lseek(cpy, 0, SEEK_SET);          // move I/O on all 3 file descriptors!
    write(22, "This is a fine ", 15); // Write starting from 0-pos
}

/*
printf("File descriptor number: %d\n", fd);     // Print fd number
int fd = open("file.txt", O_CREATE);            // file created
int fd = open("file.txt", O_WRONLY);            // file write only
int fd = open("file.txt", O_RDONLY);            // file read only
int fd = open("file.txt", O_RDONLY | O_CREATE); // possiamo parallelizzare gli argomenti
*/
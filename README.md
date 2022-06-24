# Laboratorio Sistemi operativi
Qualche esempio su ogni argomento del corso

## Comando per la compilazione
```bash
gcc -std=gnu90 main.c -o $(NAME)
```

### Threads
Per la compilazione dei thread è necessario aggiungere il flag -pthread, ad esempio:
```bash
gcc -std=gnu90 -o program main.c -pthread
```
## Librerie e loro utilizzo
```C
//Classiche
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Randomizer numeri
#include <time.h>

//Fork ed altre Sys calls
#include <unistd.h>

//pipes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//Wait
#include <sys/wait.h>

//Signals
#include <signal.h>

//threads
#include <pthread.h>
```

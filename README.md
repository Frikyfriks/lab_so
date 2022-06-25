# Laboratorio Sistemi operativi
Qualche esempio su ogni argomento del corso

## Bash e Terminale
### Espansione aritmentica
All’interno delle doppie parentesi tonde si possono rappresentare varie espressioni matematiche inclusi assegnamenti e confronti.

```bash
(( a = 7 )) (( a++ )) (( a < 10 )) (( a = 3 <10?1:0 ))
```

### Confronti logici
#### Interi
```bash
[...] [[...]]
-eq     ==
-ne     !=
-lt     <
-le     <=
-gt     >
-ge     >=
```

#### Stringe
```bash
[...] [[...]]
 ==     ==
 !=     !=
 \<     <
 \>     >
```
occorre lasciare uno spazio per il confronto

```bash
❌ ciao=ciao
✔️ ciao = ciao
```
#### File
```bash
[[ -f /tmp/prova ]] #e' un file?
[[ -d /tmp/prova ]] #e' una cartella?
[[ -e /tmp/prova ]] #esiste?
```
#### If-else statement
```bash
if [ $i -lt 10]; then
  echo "Number less then 10"
elif [ $i -gt 20]; then
  echo "Number greater then 20"
else
  echo "Number is between 10 and 20"
fi
```

#### Loops
##### For Loops
```bash
for i in ${!lista[@]}; do
  echo ${!lista[$i]}
done
```
##### While Loops
```bash
while [[ $i < 10 ]]; do
  echo $i;
  (( i++ ))
done
```

## Makefile
### Creazione
I makefile non hanno estensione, per crearli nel bash:
``` bash
make -f makefile
```
### Formato
Formato standard di un makefile:
```makefile
    target1: target2 target3 
    rule (3)
    rule (4)
    ...
    target2: target3
    rule (1)
    target3:
    rule (2)
```
### Esecuzione
L’esecuzione di un azione:
```bash
  make target1    
```
### Esempi
[Esempi di Makefile](https://github.com/Frikyfriks/lab_so/tree/main/makefile)

## Programmi C
### Comando per la compilazione
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

//Pipes
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//Wait
#include <sys/wait.h>

//Signals
#include <signal.h>

//MessageQueues
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//Threads
#include <pthread.h>
```


# Laboratorio Sistemi operativi
Qualche esempio su ogni argomento del corso

## Bash e Terminale
### Esecuzione Script
Per rendere eseguibile uno script bash, deve avere una riga commentata in alto, un metacommento (detto hash-bang o she-bang)
```bash
#!/bin/bash
```
Inoltre per far capire al sistema che è un eseguibile
```cli
chmod 777
```
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
#### Variabili
```bash
$HOSTNAME #il nome del computer host 
$HOSTYPE #il tipo di computer host 
$OSTYPE #il tipo di sistema operativo 
$LOGNAME #il nome di login dell'utente 
$HOME #la directory dell'utente 
$PATH #il percorso della directory corrente 
$HELL #la shell
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
### If else Statement

```bash
if [ -z "$(APP_NAME)" ]; then \
        echo "Empty"; \
    else \
        echo "Not empty"; \
    fi
```

### Esempi
[Esempi di Makefile](https://github.com/Frikyfriks/lab_so/tree/main/makefile)

## Programmi C
### Cheatsheet
[Cheatsheet](http://dcjtech.info/wp-content/uploads/2017/09/C-Programming.pdf)

### Comando per la compilazione
```bash
gcc -std=gnu90 main.c -o $(NAME)
```
### Signals
#### Lista di segnali
![image](https://user-images.githubusercontent.com/37717224/175788664-c18d619c-2349-46a2-bfc6-396657dd8cd9.png)

### Other Syscalls
#### Exec
##### Tipi di exec
```C
int execvp (args[0], args);
```
Le exec con la 'v' (execv/execvp/execve) hanno un parametro che è un array di argomenti (l'ultimo elemento dell'array deve essere NULL).
Le exec con la 'l' (execl/execlp/execle) prendono un numero variabile di argomenti (l'ultimo argomento passato deve essere un NULL).
Le execX prendono come primo parametro un file da lanciare.
Le execXp sono in grado di cercare l'eseguibile secondo gli stessi criteri della shell se non viene specificato un path nel file.
Le execXe hanno un parametro che è un array di stringhe che contiene le variabili di ambiente da passare al processo.

#### DUP
```C
int dup(int oldfd);
int dup2(int oldfd, int newfd);
```
La dup() prende il primo descrittore libero, ci copia dentro il descrittore oldfd e lo ritorna. La funzione dup2() copia il descrittore oldfd sul descrittore newfd: se newfd era un descrittore aperto, lo chiude prima. Entrambe le funzioni ritornano il valore del descrittore in cui oldfd è stato copiato: nel caso della dup, viene ritornato il valore del nuovo descrittore; nel caso della dup2() viene ritornato il valore di newfd.

Le funzioni dup() vengono utilizzate principalmente per redirigere lo stdout e lo stdin di un processo su un file. Vedremo nel prossimo capitolo come la shell implementa le operazioni di re-direzione dell'input e dell'output di un processo tramite queste due funzioni.

### Fork
#### Return
| Return value | Meaning   |
|--------------|-----------|
| <0           | Errore    |
| 0            | Return del Figlio  |
| >0           | Return del padre, il PID del figlio  |

#### Identificativi Processo
| ID      | Meaning   |
|--------------|-----------|
| PID     |Process ID |
| PPID    |Parent Process ID |
| SID     |Session ID |
| UID     | User ID  |
| PGID    |Process group ID |
| EUID    | Effective User ID  |

### Threads
#### Compilazione
Per la compilazione dei thread è necessario aggiungere il flag -pthread, ad esempio:
```bash
gcc -std=gnu90 -o $(NAME) main.c -pthread
```
### Librerie e loro utilizzo
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


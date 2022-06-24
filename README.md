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

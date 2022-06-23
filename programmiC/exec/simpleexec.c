#include <unistd.h>
#include <stdio.h>

int main()
{
	//comandi che metteremmo in command line + 0 per la terminazione
	char *command[] = {"grep","-E","c$","-",0};
	//File binario da eseguire
	char *bin_file = command [0];
	//Gestione dell'errore
	if (execvp(bin_file, command) == -1){
		fprintf(stderr, "Error executing %s\n", bin_file);
	}
	//Non arriverà mai qua fuori perché o switch di file oppure errore
	printf("done!");
}

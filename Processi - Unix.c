#include <stdio.h>
#include <stdlib.h>

void main(void){

	pid_t pid; int status;
	
	char *parametri[] = {"ls", 0}; // Si tratta di un puntatore a puntatori, dunque deve essere presente il terminatore o carattere nullo a termine stringa

	pid = fork();
	if (pid == -1){ // Fork restituisce -1 in caso di errore
		printf("Fork fallita\n");
		fflush(stdout);
	}
	else if(pid == 0){ // Fork restituisce in "pid" 0 al processo figlio
		printf("Processo figlio\n");
		fflush(stdout);

		execlp("ls", "ls", 0);
		execvp("ls", parametri);

		printf("La chiamata a execlp() ha fallito\n");
		fflush(stdout);
	}
	else{ // Fork restituisce il pid del figlio al processo padre
		printf("Processo padre, attesa terminazione del processo figlio\n");
		fflush(stdout);
		wait(&status);
	}



}

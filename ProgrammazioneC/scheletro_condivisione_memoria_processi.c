#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#define numChild 1

int main(void){
	char *buffer;
	pid_t pid;
	int return_value;
	// Creo una zona di memoria condivisa tra i vari processi che andrò a generare
	buffer = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	if (buffer == MAP_FAILED){
		printf("Errore nel mapping dell'area di memoria!\n");
		return -1;
	}

	for(int i = 0; i < numChild; i++){
		pid = fork();
		if (pid == 0){
			// Eseguo quello che devo eseguire
			// chiamo una funzione passando buffer, cioè l'indirizzo ella memoria condivisa in quanto il processo, e tutte le funzioni che esso andrà a chiamare, dovranno lavorare su quell'area di memoria per scambiarsi dei dati nel caso in cui dovesse essere necessaria come cosa
		}
		if (pid == -1){
			printf("Errore nella chiamata a Fork!\n");
			return -1;
		}
		else {
			continue;
		}
	}

	for (int j = 0; j < numChild; j++){
		wait(&return_value);
	}
	
	// Libero la memoria alla fine 	
	munmap(buffer, 1024);

}


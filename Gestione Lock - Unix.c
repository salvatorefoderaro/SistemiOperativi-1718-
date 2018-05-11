#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void main (void){

	// Struttura lock per la funzione 'fcntl'
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_start = 0;
	lock.l_len = 30;
	lock.l_pid = getpid();

	// Solite variabili
	char buffer[10] = "Ciao a tt!";
	int descriptor;
	
	descriptor = open("pippo2", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (descriptor == -1){
		printf("Errore nell'apertura del file!");
	}

	if (fcntl(descriptor, F_SETLKW, &lock) == -1){
		printf("Errore nella chiamata a fcntl\n");
	}

	if(write(descriptor, &buffer, strlen(buffer)) == -1){
		printf("Errore nella scrittura!\n");
	}

	printf("Premi invio per sbloccare il file...\n");
	getchar();
	
	/*
	 * Un modo è farlo in questo modo. Cioè per 'rilasciare' il file mi basta chiudere il canale
	 * close(descriptor);
	*/

	// Altrimenti questo è un altro modo, cioè scrivere nella tabella delle informazioni che rilascio il file,
	// e quindi eseguire una fcntl successiva per 'aggiornare' l'informazione per il resto del sistema
	lock.l_type = F_UNLCK;
	if (fcntl(descriptor, F_SETLKW, &lock) != -1){
		printf("File sbloccato!\n");
	}
}


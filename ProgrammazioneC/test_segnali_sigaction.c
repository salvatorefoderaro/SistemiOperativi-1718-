#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void *gestore(int numeroSegnal, siginfo_t *info, void *roba){
	printf("Ciao, gestisco il segnale!\n");
}

int main(void){

	sigset_t segnali;
	if (sigfillset(&segnali) == -1){
		printf("Errore nell'inizializzazione del set!\n");
	}

	struct sigaction gestioneSegnali;
	gestioneSegnali.sa_sigaction = (void *)gestore;
	gestioneSegnali.sa_mask = segnali;
	gestioneSegnali.sa_flags = SA_SIGINFO;
	gestioneSegnali.sa_restorer = NULL;

	if (sigaction(2, &gestioneSegnali, NULL) == -1){
		printf("Errore nell'impostazione del gestore dei segnali!\n");
	}

	printf("Dormo un po nell'attesa di un segnale...\n");
	pause();

	return 0;
}

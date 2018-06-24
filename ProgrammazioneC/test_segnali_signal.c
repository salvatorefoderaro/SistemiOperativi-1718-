#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void *gestore_segnale(int segnale_ricevuto){

	printf("Sto gestendo il segnale...\n");

}

int main(void){

	if(signal(2, (void *)gestore_segnale) == SIG_ERR){
	
		printf("Errore nell'impostazione del gestore del segnale!\n");
	}

	pause();

	printf("Hello world!\n");

	return 0;
}

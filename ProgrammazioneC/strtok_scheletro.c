#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int strtok_file(char *file_name){
	char *token;
	const char *dem = " ";
	FILE *toOpen;
	
	// Solite precauzioni sull'apertura del file
	toOpen = fopen(file_name, "r");
	if (toOpen == NULL){
		printf("Errore nell'apertura del file!\n");
		return -1;
	}
	char *buffer = NULL;
	size_t buffer_len = 0;
	// Leggo fin quando non arrivo alla fine del file	
	while(getline(&buffer, &buffer_len, toOpen) != -1){
		// Ottengo il primo token, quindi già su questo posso lavorare
		token = strtok(buffer, dem);
		// Ottengo tutti gli altri token, lavorare anche su questi. Per esempio per ogni Token richiamare la funzione 'Is_ASCII' e fare i vari controlli che dovessero servire
		while (token != NULL){
			printf("Token: %s\n", token);
			token =	strtok(NULL, dem);
		}
	}
	return 0;
}


int main () {
	strtok_file("file_prova.txt");
   return(0);
}

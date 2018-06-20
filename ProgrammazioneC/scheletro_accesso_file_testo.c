#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define fflushh(stdin) while(getchar() != '\n')

int is_ASCII(char carattere_controllo){
	// Controllo che il carattere in input sia ASCII.
	// In caso positivo restituisco 1, altrimenti 0
	if (carattere_controllo >= 'a' && carattere_controllo <= 'z'){
		return 1;
	}
	if (carattere_controllo >= 'A' && carattere_controllo <= 'Z'){
		return 1;
	}
		return 0;
}

int operazioni_file_testo(void){
	// Dichiaro le variabili che vado ad utilizzare
	char *buffer = NULL;
	size_t len = 0;
	FILE *fileDaUsare;
	
	// Inizializzo il file da utilizzare, e controllo che
	// l'apertura sia andata a buon fine, altrimenti ritorno
	// il controllo al chiamante
	fileDaUsare = fopen("Nome_del_file", "r");
	if (fileDaUsare == NULL){
		printf("Errore nell'apertura del file!\n");
		return -1;
	}

	// Se non ci sono stati problemi nell'apertura dal file,
	// allora procedo alla lettura linea per linea, eseguendo
	// tutte le operazioni che mi servono
	while(getline(&buffer, &len, fileDaUsare)){
		// All'interno di 'buffer' vengono caricate le linee di testo lette all'interno del file	

	}
	fclose(fileDaUsare);
	return 0;
}


int main(void){
	
	return 0;

}

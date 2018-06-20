#include <stdio.h>
#include <stdlibh.>
#include <stdio.h>
#define fflush(stdin) while(getchar() != '\n')
int apri_file(char *file_name){

	char *buffer;
	FILE *file_to_open;
	file_to_open = fopen(file_to_open, "r");
	if (file_to_open == NULL){
		printf("Errore nell'apertura del file!\n");
		return -1;
	}
	
	// Il teipo di elemento deve essere conforme a quello che leggo
	// Se voglio leggere una struttura, all'ora l'indirizzo di destinazione della read deve essere una struttura, e devo leggere un numer di Byte pari alla dimensione della struttura
	while(fread(tipo_elemento, sizeof(tipo_elemento), 1, file_to_open) != -1){
		// Fai qualcosa
		}
	fclose(file_to_open);

}

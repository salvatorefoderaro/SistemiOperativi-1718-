#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Da ricordare che il Read Lock non è esclusivo, nel senso che più processi contemporaneamente lo possono effettuare.
// Se è presente un Read Lock su una porzione di file, un processo non può andare a scrivere la sopra.
// Il Write Lock invece non permette ad altri processi ne di leggere, ne di scrivere. 

int main(void){
	
	int file_descriptor;
	file_descriptor = open("test.txt", O_RDWR);
	if (file_descriptor == -1){
		printf("Errore nell'apertura del file!\n");
		return -1;
	}

	struct flock blocco;
	blocco.l_type = F_RDLCK;
	blocco.l_whence = SEEK_SET;
	blocco.l_start = 0;
	blocco.l_len = 20;
	blocco.l_pid = getpid();

	if (fcntl(file_descriptor, F_SETLKW, &blocco) == -1){
		printf("Errore nel blocco del file!\n");
		return -1;
	} 

	printf("Hello world!\nSono riuscito ad acquisire il blocco del file! Ora lo sblocco...\n\n");
	getchar();

	blocco.l_type = F_UNLCK;
	if (fcntl(file_descriptor, F_SETLK, &blocco) == -1){
		printf("Errore nella rimozione del blocco!\n");
	}

}

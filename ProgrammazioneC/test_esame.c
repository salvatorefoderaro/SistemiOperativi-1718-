#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int controllo(int numero, void *buffer){
	int contatore = 0;
	char *bufferr = buffer;
	printf("Il valore è: %c\n", *(bufferr+10) );
}

int main(void){

	pid_t pids[256];
	int k = 0;
	char carattere;
	int fd = open("testt", O_RDWR);
	while (k<10){
	read(fd, &carattere, 1);
	printf("Carattere è: %c e %u\n\n", carattere, carattere);
	k = k +1;
	}
	int status;
	void *address = mmap(NULL, 4096 , PROT_READ, MAP_SHARED, fd, 0);
	if (address == MAP_FAILED){
		printf("Errore nel mapping della memoria!");
	}

	for (int i = 0; i<2; i++){
		pids[i] = fork();
		if(pids[i] == 0){
		controllo(i, address);
			exit(20);
		}
		if(pids[i] == -1){
			printf("Errore nella chiamata a fork!\n");
		}
		continue;
	}

	for (int j = 0; j<2; j++){
		wait(&status);
	}

	close(fd);
	printf("Ci siamo!\n");
	return 10;
}

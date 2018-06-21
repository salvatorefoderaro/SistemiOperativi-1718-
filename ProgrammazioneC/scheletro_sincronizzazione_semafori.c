

#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){

	sem_t *semaforo = malloc(sizeof(sem_t));
	if (sem_init(semaforo, 1, 1) == -1){
		printf("Errore nell'inizializzazione del semaforo!\n");
		return -1;
	}
	printf("Cerco di prendere il controllo del semaforo...\n");
	sem_wait(semaforo);
	printf("Preso il controllo del semaforo! Ora lo libero...\n");
	sem_post(semaforo);
	printf("Hello world!");
	free(semaforo);
	return 0;
}

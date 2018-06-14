#include <stdio.h>
#include <stdlib.h>
#define NUM_CHILD 26
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>


sem_t semaphore;
struct massimo {
	int maxValue;
	char maxIs[1024];
};

int check_if_ASCII(int carattere){
		if ('a' <= carattere && 'z' >= carattere){
		return 1;
	}

	if ('A' <= carattere && 'Z' >= carattere){
		return 1;
	}else{

	return 0;

	}
	}

int controllo_lettere(char *nome_file, int valore_i, struct massimo *max){
	char controllo_minuscola = 'a' + valore_i;
	char controllo_maiuscola = 'A' + valore_i;
	FILE *controllo;
	char nome_file_testo[100];
	char *buffer = NULL;
	int counter = 0;
	int spazio = 1;
	size_t valore = 0;
	ssize_t value;

	sprintf(nome_file_testo, "%s", nome_file);
        
	controllo = fopen(nome_file_testo, "r");
	
	if (controllo == NULL){
		printf("Errore nell'apertura del file!");
		exit(-1);
	}
	
	while ((value = getline(&buffer, &valore, controllo)) != -1){
		for (int j = 0; j < strlen(buffer); j++){
			if (check_if_ASCII(buffer[j])){
				if(buffer[j] != ' ' && spazio != 0 ){
					if(buffer[j] == controllo_minuscola || buffer[j] == controllo_maiuscola){
					counter = counter + 1;
					spazio = 0;
					}
				}
				if(buffer[j] == ' '){
					spazio = 1;
				}else{
				continue;
				}	
			}
		}
		spazio = 1;
	}

	sem_wait(&semaphore);
	if (counter > max->maxValue){
		max->maxValue = counter;
		sprintf(max->maxIs, "%c", controllo_minuscola);
		
	}
	if (counter == max->maxValue){
		sprintf(max->maxIs, "%s, %c", max->maxIs, controllo_minuscola);
	}
	sem_post(&semaphore);

	
	fclose(controllo);
	return 10;
}

int main(int argc, char *argv[]){
	struct massimo *max = mmap(NULL, sizeof(struct massimo), PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (max == MAP_FAILED){
		printf("Errore nel mapping!\n");
		return -2;	
	}
	if (sem_init(&semaphore, 0, 1) == -1){
		printf("Errore nella creazione del semaforo!");
	}
	max->maxValue = 0;
	sprintf(max->maxIs, " ");
	pid_t pid;
	int valore_ritorno;

	for (int i = 0; i < NUM_CHILD; i++){
		pid = fork();
		if (pid == -1){
			printf("Errore nella chiamata a Fork!\n");
		}
		if (pid == 0){
		controllo_lettere(argv[1], i, max);
		exit(1);
		}
		else{
			wait(&valore_ritorno);
		}

	}

	printf("Il valore del carattere massimo è: %d. La lista dei caratteri massimi è: %s\n", max->maxValue, max->maxIs);
}

#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	
	cpu_set_t *set = malloc(sizeof(cpu_set_t));
	if (set == NULL){
		printf("Errore nell'allocazione della memoria!\n");
	}
	CPU_ZERO(set);
	CPU_SET(0, set);
	CPU_SET(1, set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), set) == -1){
		printf("Errore nell'impostazione dell'affinita!\n");
	}
	if (sched_getaffinity(0, sizeof(cpu_set_t), set) == 0){
		printf("Il numero di CPU è: %d\n", CPU_COUNT(set));
	}
	printf("Hello world!\n");
	free(set);
	return 0;

}

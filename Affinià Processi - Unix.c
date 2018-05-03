#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

void *modificaAffinita(){
	
	int scelta; cpu_set_t c; pid_t pid;

	if (sched_getaffinity(0,sizeof(cpu_set_t),&c) == 0){
	printf("\nCiao! Il tuo PID è %d! Scegli cosa fare.\nRimuovere tutte le CPU (0), aggiungerne solamente una (1), rimuoverne una (2) o vedere l'attuale maschera (3). Altrimenti inserisci (1000) per uscire!\n",getpid());

	while (scelta != 1000){
		printf("\nCosa vuoi fare?\n");
		fflush(stdout);
		scanf("%d", &scelta);
		while((getchar())!= '\n');
		
		switch (scelta){
		case 0:
				CPU_ZERO(&c);
				if(sched_setaffinity(0,(size_t)1,&c) == 0){
					printf("Affinità modificata con successo!\n");
				} else {
					printf("Non è possibile assegnare nessuna CPU alla maschera!\n");
				}
				break;

			case 1:
				printf("Quale CPU vuoi aggiungere?\n");
				scanf("%d", &scelta);
				while((getchar()) != '\n');
				CPU_SET(scelta, &c);
				if(sched_setaffinity(getpid(),(size_t)1, &c) == 0){
					printf("Affinità modificata con successo!\n");
				}	
				break;

			case 2:
				printf("Quale CPU vuoi rimuovere?\n");
				scanf("%d", &scelta);
				while((getchar())!='\n');
				CPU_CLR(scelta, &c);
				if(sched_setaffinity(getpid(),(size_t)1, &c) == 0){
				printf("Affinità modificata con successo!\n");
				}
				break;

			case 3:	
				
				printf("Il numero CPU attualmente presenti nel gruppo è: %d. Mentre la maschera attuale è: %lx\n", CPU_COUNT(&c), *(c.__bits));
				break;
				
				//printf("La maschera attuale è: %lx\n", c);	
	
				/* char *comando;
				asprintf(&comando, "taskset -p %u", getpid());		
				system(comando);
				*/

				/* pid_t controlloAffinita;
				controlloAffinita = fork();
				if(controlloAffinita == 0){
				printf("Weee");
				execlp("/bin/bash","bash", "-c",comando,(char*)0);
					}
				*/       	
		
		}
		}
		pthread_exit((void*)100);
	}
}

int main(void){
	pid_t a; pthread_t threadd; void *ritorno;
	a = getpid();
	cpu_set_t b; // cpu_set_t è la struttura dati che devo utilizzare

	size_t c = 2;
	if (pthread_create(&threadd, NULL, modificaAffinita, NULL) == 0){
		printf("Thread creato con successo! Codice del thread %lu\n", threadd);
	};
	pthread_join(threadd, &ritorno);
	
	return 0;
}

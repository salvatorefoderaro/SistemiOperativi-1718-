#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>

 struct threadArg{
	pthread_attr_t threadAttr;
	cpu_set_t cpuAffinity;
	};
		
void *modificaAffinitaProcesso(){
	
	int scelta; cpu_set_t c; pid_t pid;

	if (sched_getaffinity(0,sizeof(cpu_set_t),&c) == 0){
	printf("\nCiao! Il tuo PID è %d! Stai lavorando con l'affinità del processo! Scegli cosa fare.\nRimuovere tutte le CPU (0), aggiungerne solamente una (1), rimuoverne una (2) o vedere l'attuale maschera (3). Altrimenti inserisci (1000) per uscire!\n",getpid());

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
				
		}
		}
		pthread_exit((void*)100);
	}
}

void *modificaAffinitaThread(struct threadArg *arguments){
	
	struct threadArg *struttura_argomenti = arguments;
	
	int scelta; cpu_set_t c;
	
	if (pthread_attr_setaffinity_np(&(struttura_argomenti->threadAttr), sizeof(struttura_argomenti->cpuAffinity), &(struttura_argomenti->cpuAffinity)) != 0){
			printf("Settaggio affinità thread fallita!");
		}
	
	printf("\nCiao! Il tuo PID è %d! Stai lavorando con l'affinità del Thread! Scegli cosa fare.\nRimuovere tutte le CPU (0), aggiungerne solamente una (1), rimuoverne una (2) o vedere l'attuale maschera (3). Altrimenti inserisci (1000) per uscire!\n",getpid());

	while (scelta != 1000){
		printf("\nCosa vuoi fare?\n");
		fflush(stdout);
		scanf("%d", &scelta);
		while((getchar())!= '\n');
		
		switch (scelta){
		case 0:
				CPU_ZERO(&c);
				if(pthread_attr_setaffinity_np(&(struttura_argomenti->threadAttr), sizeof(struttura_argomenti->cpuAffinity), &(struttura_argomenti->cpuAffinity)) == 0){
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
				if(pthread_attr_setaffinity_np(&(struttura_argomenti->threadAttr), sizeof(struttura_argomenti->cpuAffinity), &(struttura_argomenti->cpuAffinity)) == 0){
					printf("Affinità modificata con successo!\n");
				}	
				break;

			case 2:
				printf("Quale CPU vuoi rimuovere?\n");
				scanf("%d", &scelta);
				while((getchar())!='\n');
				CPU_CLR(scelta, &c);
				if(pthread_attr_setaffinity_np(&(struttura_argomenti->threadAttr), sizeof(struttura_argomenti->cpuAffinity), &(struttura_argomenti->cpuAffinity)) == 0){
				printf("Affinità modificata con successo!\n");
				}
				break;

			case 3:	
				c = struttura_argomenti->cpuAffinity;	
				printf("Il numero CPU attualmente presenti nel gruppo è: %d. Mentre la maschera attuale è: %lx\n", CPU_COUNT(&c), *(c.__bits));
				break;
    	
			}
		}
		pthread_exit((void*)100);
	}

int main(void){
	pthread_t thread1;
	pthread_t thread2; void *ritorno;
	pthread_attr_t attributiThread; cpu_set_t threadAffinity;
	
	CPU_ZERO(&threadAffinity);
	CPU_SET(0, &threadAffinity);
	pthread_attr_init(&attributiThread);
	
	struct threadArg argomenti;
	struct threadArg *argomentiPunt = &argomenti;
	argomentiPunt -> threadAttr = attributiThread;
	argomentiPunt -> cpuAffinity = threadAffinity;
	
	if (pthread_attr_setaffinity_np(&attributiThread, sizeof(threadAffinity), &threadAffinity) != 0){
			printf("L'impostazione dell'affinità non è andata a buon termine");
		};

	if (pthread_create(&thread1, &threadAffinity, (void *)modificaAffinitaThread, argomentiPunt) == 0){
		printf("\nThread creato con successo! Codice del thread %lu\n", thread1);
	}; 
	
	pthread_join(thread1, &ritorno);
	
	if (pthread_create(&thread2, NULL, modificaAffinitaProcesso, NULL) == 0){
		printf("\nThread creato con successo! Codice del thread %lu\n", thread1);
	}; 
		
	pthread_join(thread2, &ritorno);

	
	return 0;
}

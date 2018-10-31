#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

void *thread(void *ptr){
	
	

	int exitCode = 100;
	printf("Ciao, sono un thread! Questo è l'argomento passato: %s\n", (char *)ptr);
	// Anche in questo caso come su Windows devo castare, in quanto lavoro con un tipo di dato generico
	sleep(2);
	printf("Il mio codice identificativo è: %d", pthread_self());
	pthread_exit(&exitCode);
}

void main(void){
	char *parametro[] = {"hellouuu", "ciao", "a", "tuttiii"};
	char *paramentro = "ciao";
	pthread_t threadInfo; void *exit;
	pthread_attr_t treadAttribute;
	
 	pthread_create(&threadInfo, NULL, *thread, paramentro);
	
	 pthread_join(threadInfo, &exit);
	
	 printf("\n Il codice di uscita del thread è: %d", exit);
	
	sleep(2);
	
}

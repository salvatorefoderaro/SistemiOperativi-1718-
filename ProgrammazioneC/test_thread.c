#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

__thread int id;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *funzioneThread(void *argomentoFunzione){
	printf("Cerco di prendere il controllo del mutex...\n");
	pthread_mutex_lock(&mutex);
	printf("Ho preso il controllo del Mutex! Ora lo libero...\n");
	pthread_mutex_unlock(&mutex);
	char *stringa;
	stringa = argomentoFunzione;
	printf("We, 'bell stu Thread!\nL'argomento passato al thread è: %s\n", stringa);
	pthread_exit((void*)10);
}

int main(void){

	pthread_t thread;
	if (pthread_mutex_init(&mutex, NULL) != 0){
		printf("Errore nell'inizializzazione del mutex!\n");
		return -1;
	}	
	char *stringa = "Wee belli";
	int *return_value;
		
	if (pthread_create(&thread, NULL, funzioneThread, (void *)stringa) != 0){
	
	printf("Errore nella creazione del thread!\n");

	} 
	
	pthread_join(thread, (void**)&return_value);

	printf("Hello world!\n");

	return 0;

}

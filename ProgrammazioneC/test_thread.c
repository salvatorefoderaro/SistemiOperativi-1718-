#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

__thread int id;

void *funzioneThread(void *argomentoFunzione){
	
	char *stringa;
	stringa = argomentoFunzione;
	printf("We, 'bell stu Thread!\nL'argomento passato al thread è: %s\n", stringa);
	//pthread_exit((void*)10);
	return (void*)10;
}

int main(void){

	pthread_t thread;
	
	char *stringa = "Wee belli";
	int *return_value;
		
	if (pthread_create(&thread, NULL, funzioneThread, (void *)stringa) != 0){
	
	printf("Errore nella creazione del thread!\n");

	} 
	
	pthread_join(thread, (void**)&return_value);

	printf("Hello world!\n");

	return 0;

}

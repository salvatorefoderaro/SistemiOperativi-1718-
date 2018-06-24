#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	char c;
	void *pointer;
	
	pointer = sbrk(0);
	sbrk(1);
	// Anche con un incremento di solamente 1, viene comunque allocata un'intera pagina di memoria
	for(i = 0;;i++){
	printf("%d | Cercando di accedere... %x\n",i, (unsigned int)(pointer+i));
	printf("Hello world!\n");
	c = *(char*)(pointer+i);
	
	i = i+1;
	}
}

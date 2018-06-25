#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){

	int fd;
	fd = open("test.txt", O_RDWR);
	if (fd == -1){
		printf("Errore nell'apertura del file!\n");	
	}
	char buffer[1024];
	while(read(fd, (void*)buffer, sizeof(buffer)) != 0){
		printf("Sto leggendo!\n\n");
		printf("Ho letto questo:\n\n%s", buffer);

	}

	printf("Hello world!\n");
}






#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void main(void){

	//if(link("Processi - Unix.c", "Cartella_Prova/eeee.c") == -1){
	//	printf("Linking fallito!");	
	//}

	//if(unlink("Cartella_Prova/eeee.c") == -1){
	//	printf("Unlincking fallito!");
	//}
	

	if(symlink("Processi - Unix.c", "../eeeee1111") == -1){
		printf("Creazione link simbolico fallita!\n");
	}
}

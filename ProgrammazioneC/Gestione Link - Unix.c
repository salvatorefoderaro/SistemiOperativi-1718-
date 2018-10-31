




#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void main(void){

	if(link("Processi - Unix.c", "../eeee.c") == -1){
	printf("Linking fallito!");	
	}
	char path_completo[300];
	getwd(path_completo);
	strcat(path_completo, "/Processi - Unix.c");
	printf("Il path completo del file è: %s", path_completo);

	//if(unlink("Cartella_Prova/eeee.c") == -1){
	//	printf("Unlincking fallito!");
	//}
	

	if(symlink(path_completo, "../eeeee1sssss111sdsds.c") == -1){
		printf("Creazione link simbolico fallita!\n");
	}
}

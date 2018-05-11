#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
void main(){
	
	int scelta; char nomeFile[100]; int descrittore; char buffer[1024];
	int dimensioneScritta; int posizioneScrittura; char nomeNuovoFile[100]; int descrittoreCopia;

	printf("Ciao! Cosa vuoi fare?\nAprire un nuovo file (0), creare un nuovo file (1), scrivere sul file aperto (2), copiare il file aperto in un nuovo file (3), eliminare il contenuto del file (4), chiudere il file (5), visualizzare il contenuto del file (6) o terminare (7)\n");
	while(scelta != 7){
			printf("\nCosa vuoi fare?\n\n");
			scanf("%d",&scelta);
			while((getchar()) != '\n');
			
			if(scelta == 0){
				printf("Quale file vuoi aprire?\n");
				scanf("%s",nomeFile);
				descrittore = open(nomeFile, O_RDWR);
				if(descrittore == -1){
					printf("Errore nell'apertura del file\n");
				}
			}
			
			if(scelta == 1){
				printf("Che nome vuoi dare al file da creare?\n");
				scanf("%s", nomeFile);
				while((getchar()) != '\n');
				descrittore = creat(nomeFile, 0666);
				if(descrittore == -1){
					printf("Errore nella creazione del file!\n");
				}
			}
			
			if(scelta == 2){
				printf("Da che posizione vuoi iniziare a scrivere all'interno del file?\n");
				scanf("%d", &posizioneScrittura);
				while((getchar()) != '\n');
				printf("Cosa vuoi scrivere all'interno del file?\n");
				if(lseek(descrittore, 0, posizioneScrittura) == -1){
						printf("Posizionamento all'interno del file fallito!\n");
				}
				gets(buffer);
								dimensioneScritta = write(descrittore, buffer, strlen(buffer));
				if(dimensioneScritta == -1){
					printf("Errore nella scrittura sul file!\n");
				}
			}

			if(scelta == 3)
			{
				printf("In che nuovo file vuoi copiare il contenuto del file aperto? (File esistente, nel caso di un file non esistente verrà creato automaticamnte!\n");
				scanf("%s", nomeNuovoFile);
				while((getchar()) != '\n');
				descrittoreCopia = open(nomeNuovoFile, O_CREAT | O_WRONLY, 0666);
				if (descrittoreCopia == -1){
					printf("Apertura del file di destinazione fallita!");
					exit(-1);
				}
				int datiLetti = 10; int datiScritti = 10;
				while(datiLetti != 0 & datiLetti == datiScritti){

					datiLetti = read(descrittore, buffer, strlen(buffer));
					datiScritti = write(descrittoreCopia, buffer, strlen(buffer));
					if(datiLetti == -1 | datiScritti == -1){
						printf("Errore nella lettura/scrittura!\n");
					}	
				}
				close(descrittoreCopia);
				}
			

			if(scelta == 4){
				descrittore = open(nomeFile, O_TRUNC);
				if(descrittore == -1){
					printf("Errore nel troncamento del file!\n");
				}
			}

			if(scelta == 5){
				if(close(descrittore) == -1){
					printf("Errore nella chiusura del file!\n");
				}
			}

			if(scelta == 6){	
				 pid_t pid;
				pid = fork();
				if(pid == 0){
					execlp("less", "less", nomeFile, "\0");
				}
			}
		}	



}

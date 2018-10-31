#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


struct comunicazione {
	int operazione;
	int valore_ritorno;
	char argomento1[512];
	char argomento2[512];
	};

void *gestore_utente(){
	
	struct comunicazione session;
	int valore_ritorno;
	int scelta;
	
	while(1){
	printf("\n\nQuale operazione vuoi eseguire?\n0 - Inserisci un nuovo messaggio\n1 - Leggi tutti i messaggi presenti\n2 - Effettua l'accesso al sistema\n3 - Elimina un messaggio\n4 - Termina l'esecuzione del programma\n\nInserisci il numero del comando che vuoi eseguire: ");
		
	scanf("%d", &scelta);
	while(getchar() != '\n');
	switch (scelta) {
	
	case 0: // Inserimento nuovo messaggio
	
		printf("Quale messaggio vuoi inserire?\n");
		fgets(session.argomento1, 512, stdin);
		printf("Quale oggetto vuoi inserire?\n");
		fgets (session.argomento2, 128, stdin);
		if(send(sock, &session, sizeof(struct session) , 0) < 0)
		{
			printf("Errore nell'invio!\n");   
			break;
		}
		if(recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf("Errore nella ricezione!\n");
			break;
		}
		break;
	
	case 1:
		leggi_tutti_messaggi();	
			
	case 2: // Login
		session.operazione = 1;
		session.valore_ritorno = 0;
		printf("\nInserisci il nome utente\n");
		scanf("%s", session.argomento1);
		printf("Inserisci la password\n");
		scanf("%s", session.argomento2);
		
		if(send(sock, &session, sizeof(struct session) , 0) < 0)
		{
			printf("Errore nell'invio!\n");   
			break;
		}
		if( recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf("Errore nella ricezione!\n");
			break;
		}
		if (valore_ritorno == 0){
			printf("Accesso effettuato correttamente!");
			break;
			}
		if (valore_ritorno == -1){
			printf("Errore nell'operazione...\n");
			break;
			}
		if (valore_ritorno == -2){
			printf("Nome utente o password errati\n");
			break;
			}
	
	case 3: // Elimina messaggio
		session.operazione = 3;
		session.valore_ritorno = 0;
		printf("Quale messaggio vuoi eliminare?\n");
		scanf("%d", session.argomento1);
		session.argomento2 = NULL;
		
		if(send(sock, &session, sizeof(struct session) , 0) < 0)
		{
			printf("Errore nell'invio!\n");   
			break;
		}
		
		if( recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf("Errore nella ricezione!\n");
			break;
		}
		if (valore_ritorno == 0){
			printf("Messaggio eliminato correttamente!");
			break;
			}
		
		if (valore_ritorno == -1){
			printf("Errore da parte del server...");
			break;
			}
		
		if (valore_ritorno == -2){
			printf("Devi effettuare il login prima di eseguire quest'operazione!\n");
			break;
			}
				
	case 4: // Termina esecuzione programma
		pthread_exit(&valore_ritorno);
	}
	}
	}

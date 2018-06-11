
	
/*
    C ECHO client example using sockets
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr

struct comunicazione {
	int operazione;
	int valore_ritorno;
	char argomento1[512];
	char argomento2[512];
	};
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 6000 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
    
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
		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
		{
			printf("Errore nell'invio!\n");   
			break;
		}
		if(recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf("Errore nella ricezione!\n");
			break;
		}
		if(valore_ritorno == -20){
			printf("Effettuare prima l'accesso!\nPremi un testo per continuare...");
			getchar();
			break;
			}
		break;
	
	case 1:
		// leggi_tutti_messaggi();	
			
	case 2: // Login
		session.operazione = 2;
		session.valore_ritorno = 0;
		printf("\nInserisci il nome utente\n");
		scanf("%s", session.argomento1);
		printf("Inserisci la password\n");
		scanf("%s", session.argomento2);
		
		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
		{
			printf("Errore nell'invio!\n");   
			break;
		}
		if(recv(sock, &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf("Errore nella ricezione!\n");
			break;
		}
		if (valore_ritorno > 0){
			printf("Accesso effettuato correttamente!");
			break;
			}

		if (valore_ritorno < 0){
			printf("Errore da qualche parte!");
			break;
			}
	
	case 3: // Elimina messaggio
		session.operazione = 3;
		session.valore_ritorno = 0;
		printf("Quale messaggio vuoi eliminare?\n");
		scanf("%d", session.argomento1);
		strcpy(session.argomento2, NULL);
		
		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
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
     
    close(sock);
    return 0;
 }
    




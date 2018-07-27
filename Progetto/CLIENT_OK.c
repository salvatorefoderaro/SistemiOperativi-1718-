#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#define doppio_login "Il login è stato già effettuato!"
#define errore_sessione "Per eseguire quest'operazione, effettua prima l'accesso!"
#define errore_login "Username o password errati!"
#define errore_comunicazione "Errore nella comunicazione con il server!"

	struct comunicazione {
	int operazione;
	int valore_ritorno;
	char argomento1[512];
	char argomento2[512];
	};
	
	struct messaggi {
	int id_utente;
	int id_messaggio;
	char messaggio[512];
	char oggetto[128];
	char mittente[64];
	};
	
void login(int sock){
		struct comunicazione session;
		int valore_ritorno;
		session.operazione = 0;
		printf("\nInserisci il nome utente\n");
		scanf("%s", session.argomento1);
		printf("\nInserisci la password\n");
		scanf("%s", session.argomento2);
		
		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
		{
			printf(errore_comunicazione);   
		}
		if(recv(sock, &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf(errore_comunicazione);
		}
		if (valore_ritorno > 0){
			printf("\nAccesso effettuato correttamente!\n");
			return;
			}

		if (valore_ritorno == -1){
			printf(doppio_login);
			return;
			}
			
		if (valore_ritorno == -3){
			printf(errore_login);
			return;
			}
			
		else{
			printf("\nErrore nella richiesta. Codice di errore: %d\n", valore_ritorno);
			return;
			}
	
	}

void insert_message(int sock){
		struct comunicazione session;
		int valore_ritorno;
		session.operazione = 2;
		printf("\nQuale messaggio vuoi inserire?\n");
		fgets(session.argomento1, 512, stdin);
		printf("\nQuale oggetto vuoi inserire?\n");
		fgets (session.argomento2, 128, stdin);
		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
		{
			printf(errore_comunicazione);   
			exit(-1);
		}
		if(recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf(errore_comunicazione);
			exit(-1);
		}
		if(valore_ritorno == -2){
			printf(errore_sessione);
			exit(-1);}
			
		if(valore_ritorno > 0){
			printf("\nIl messaggio è stato inserito correttamente!\n");
			return;

		}
		else{
			printf("\nErrore nella richiesta. Codice di errore: %d\n", valore_ritorno);
			return;
			}
	}

void delete_message(int sock){
		struct comunicazione session;
		int valore_ritorno;
		session.operazione = 3;
		printf("\nQuale messaggio vuoi eliminare?\n");
		scanf("%d", &(session.valore_ritorno));
		
		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
		{
			printf(errore_comunicazione);
			return;
		}
		
		if( recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf(errore_comunicazione);
			return;

		}
		if (valore_ritorno > 0){
			printf(\n"\nMessaggio eliminato correttamente!\n");
			return;

		}
		if (valore_ritorno == -2){
			printf(errore_sessione);
			return;

		}
			
		else{
			printf("\nErrore nella richiesta. Codice di errore: %d\n", valore_ritorno);
			return;
		}
	}
	
void see_all_messages(int sock){
		struct comunicazione session;
		struct messaggi input;
		session.operazione = 1;
		session.valore_ritorno = 0;
		int valore_ritorno;
		int test;

		if(send(sock, &session, sizeof(struct comunicazione) , 0) < 0)
		{
			printf(errore_comunicazione);   
			return;

		}
		int dimensione;
		recv(sock , &dimensione , sizeof(int), 0);
		if (dimensione == 0){
				printf("\nNessun messaggio presente!\n");
			}
		while(dimensione > 0)
		{
		test  = recv(sock , &input , sizeof(struct messaggi) , 0);
		dimensione = dimensione - test;
		printf ("\nMessaggio numero: %d  | Mittente messaggio: %s | Oggetto messaggio: %s\n\n%s\n", input.id_messaggio, input.mittente, input.oggetto, input.messaggio);
		valore_ritorno = 1;
		if(send(sock, &valore_ritorno, sizeof(int) , 0) < 0)
			{
			printf(errore_comunicazione);   
			return;
			}
			}
	return;
	}
 
int main(int argc , char *argv[])
{

	int sock;
    struct sockaddr_in server;
	int scelta;
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 6000 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    printf("\n\nConnessione effettuata correttamente, benvenuto nel sistema!");
	while(1){
	printf("\nQuale operazione vuoi eseguire?\n0 - Effettua l'accesso al sistema\n1 - Leggi tutti i messaggi presenti\n2 - Inserisci un nuovo messaggio\n3 - Elimina un messaggio\nQuale operazione vuoi eseguire?\n");
		
	scanf("%d", &scelta);
	while(getchar() != '\n');
	switch (scelta) {
	
	case 0: // Login
		login(sock);
		break;
	
	case 1:
		see_all_messages(sock);
		break;
	
	case 2: // Inserimento nuovo messaggio
		insert_message(sock);
		break;
	
	case 3: // Elimina messaggio
		delete_message(sock);
		break;
				
	case 4: // Termina esecuzione programma
		close(sock);
		return 0;
		}
	}
 }

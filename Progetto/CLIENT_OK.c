#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#define fflush(stdin) while(getchar() != '\n')
#define doppio_login "\n    *****    Il login è stato già effettuato!    *****\n"
#define errore_sessione "\n    *****    Per eseguire quest'operazione, effettua prima l'accesso!    *****\n"
#define errore_login "\n    *****    Username o password errati!    *****\n"
#define errore_comunicazione "\n    *****    Errore nella comunicazione con il server!    *****\n"

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
			printf("\n    *****    Accesso effettuato correttamente!    *****\n");
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
			printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
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
			return;
		}
		if(recv(sock , &valore_ritorno , sizeof(int) , 0) < 0)
		{
			printf(errore_comunicazione);
			return;
		}
		if(valore_ritorno == -2){
			printf(errore_sessione);
			return;
			}
			
		if(valore_ritorno > 0){
			printf("\n    *****    Il messaggio è stato inserito correttamente!    *****\n");
			return;

		}
		else{
			printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
			return;
			}
	}

void delete_message(int sock){
		struct comunicazione session;
		int valore_ritorno;
		session.operazione = 3;
		printf("\nQuale messaggio vuoi eliminare?\n");
		scanf("%d", &(session.valore_ritorno));
		fflush(stdin);
		int size;
		if(size = (send(sock, &session, sizeof(struct comunicazione) , 0)) < 0)
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
			printf("\n    *****    Messaggio eliminato correttamente!    *****\n");
			return;

		}
		if (valore_ritorno == -2){
			printf(errore_sessione);
			return;
		}
		if (valore_ritorno == -6){
			printf("\n    *****    Nessun messaggio presente!    *****\n");
			return;
		}
		
		if (valore_ritorno == -8){
			printf("\n    *****    Il messaggio che vuoi eliminare non è presente!    *****\n");
			return;
		}
		
		if (valore_ritorno == -9){
			printf("\n    *****    Non hai il permesso per eliminare il messaggio!    *****\n");
			return;
		}
			
		else{
			printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
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
				printf("\n    *****    Nessun messaggio presente!    *****\n");
			}
		while(dimensione > 0)
		{
		test  = recv(sock , &input , sizeof(struct messaggi) , 0);
		dimensione = dimensione - test;
		printf("\n          ********************\n");
		printf ("\nMessaggio numero: %d  | Mittente messaggio: %s | Oggetto messaggio: %s\n%s\n", input.id_messaggio, input.mittente, input.oggetto, input.messaggio);
		printf("          ********************\n");
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
     
    server.sin_addr.s_addr = inet_addr("151.29.60.105");
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
	printf("\n0 - Effettua l'accesso al sistema\n1 - Leggi tutti i messaggi presenti\n2 - Inserisci un nuovo messaggio\n3 - Elimina un messaggio\n4 - Termina esecuzione programma\n\nQuale operazione vuoi eseguire?\n");
		
	scanf("%d", &scelta);
	fflush(stdin);
	switch (scelta) {
	
	case 0: // Login
		login(sock);
		break;
	
	case 1: // Leggi tutti i messaggi presenti
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

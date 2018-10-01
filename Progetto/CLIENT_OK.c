#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <signal.h>
#define fflush(stdin) while(getchar() != '\n')
#define doppio_login "\n    *****    Il login è stato già effettuato!    *****\n"
#define errore_sessione "\n    *****    Per eseguire quest'operazione, effettua prima l'accesso!    *****\n"
#define errore_login "\n    *****    Username o password errati!    *****\n"
#define errore_comunicazione "\n    *****    Errore nella comunicazione con il server!    *****\n\n"

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
	
	struct comunicazione session;
	char comunicazioneServer[1024];
	int valore_ritorno;
	
char *encode(struct comunicazione *struttura){
    char *buffer = malloc(sizeof(struct comunicazione)+3*sizeof(char));
    sprintf(buffer, "%d|%d|%s|%s", htons(struttura->operazione), htons(struttura->valore_ritorno),struttura->argomento1, struttura->argomento2);
    return buffer;  
}

void decodeMessage(char *buffer, struct messaggi *struttura){

   struttura->id_messaggio = ntohs(atoi(strtok(buffer, "|")));
   struttura->id_utente = ntohs(atoi(strtok(NULL, "|")));
   strcpy(struttura->messaggio, strtok(NULL, "|"));
   strcpy(struttura->mittente, strtok(NULL, "|"));
   strcpy(struttura->oggetto, strtok(NULL, "|"));
}
 
int sendThroughSocket(int socket, int size, void *buffer){
	int nBytes = send(socket, buffer, size, 0);
	if (nBytes < 1){
		printf(errore_comunicazione);
		free(buffer);
		exit(-1);
	} else if (nBytes != size){
		free(buffer);
		exit(-1);
	}
	return nBytes;
}

int receiveThroughSocket(int socket, int size, void *buffer){
	int nBytes;
	nBytes = recv(socket, buffer , size , 0);
	if (nBytes < 1){
		printf(errore_comunicazione);
		exit(-1);     
	} else if (nBytes != size){
		exit(-1);
	}
	return nBytes;
}

void login(int sock){

		session.operazione = 0;
		printf("\nInserisci il nome utente\n");
		scanf("%s", session.argomento1);
		printf("\nInserisci la password\n");
		scanf("%s", session.argomento2);
		
		char *toSend = encode(&session);
		sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
		free(toSend);
		receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
		valore_ritorno = ntohs(valore_ritorno)  - 1000;
		
		if (valore_ritorno > 0){
			printf("\n    *****    Accesso effettuato correttamente!    *****\n");
			return;
		}else if (valore_ritorno == -1){
			printf(doppio_login);
			return;
		} else if (valore_ritorno == -3){
			printf(errore_login);
			return;
		} else {
			printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
			return;
		}
}

void insert_message(int sock){
	session.operazione = 2;
	printf("\nQuale messaggio vuoi inserire?\n");
	fgets(session.argomento1, 512, stdin);
	printf("\nQuale oggetto vuoi inserire?\n");
	fgets (session.argomento2, 128, stdin);
	
	char *toSend = encode(&session);
	sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
	free(toSend);
	receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
	valore_ritorno = ntohs(valore_ritorno)  - 1000;
	
	if(valore_ritorno == -2){
		printf(errore_sessione);
		return;
	} else if(valore_ritorno > 0){
		printf("\n    *****    Il messaggio è stato inserito correttamente!    *****\n");
		return;
	} else{
		printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
		return;
	}
}

void delete_message(int sock){
	session.operazione = 3;
	printf("\nQuale messaggio vuoi eliminare?\n");
	scanf("%d", &(session.valore_ritorno));
	fflush(stdin);
	strcpy(session.argomento1, "NULL");
	strcpy(session.argomento2, "NULL");
	
	char *toSend = encode(&session);
	sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
	free(toSend);
	receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
	valore_ritorno = ntohs(valore_ritorno) - 1000;
	
	if (valore_ritorno > 0){
		printf("\n    *****    Messaggio eliminato correttamente!    *****\n");
		return;
	} else if (valore_ritorno == -2){
		printf(errore_sessione);
		return;
	} else if (valore_ritorno == -6){
		printf("\n    *****    Nessun messaggio presente!    *****\n");
		return;
	} else if (valore_ritorno == -8){
		printf("\n    *****    Il messaggio che vuoi eliminare non è presente!    *****\n");
		return;
	} else if (valore_ritorno == -9){
		printf("\n    *****    Non hai il permesso per eliminare il messaggio!    *****\n");
		return;
	} else{
		printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
		return;
	}
}
	
void see_all_messages(int sock){
	struct messaggi input;
	session.operazione = 1;
	session.valore_ritorno = 0;
	strcpy(session.argomento1, "NULL");
	strcpy(session.argomento2, "NULL");
	
	char *toSend = encode(&session);
	sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
	free(toSend);
	
	int dimensione;
	receiveThroughSocket(sock, sizeof(int), &dimensione);
	dimensione = ntohs(dimensione);
	if (dimensione == 0){
		printf("\n    *****    Nessun messaggio presente!    *****\n");
	}
	char *toReceive = malloc(sizeof(struct messaggi)+4*sizeof(char));
	while(dimensione > 0){
		int dataReceived  = receiveThroughSocket(sock, sizeof(struct messaggi)+4*sizeof(char), toReceive);
		decodeMessage(toReceive, &input);
		dimensione = dimensione - dataReceived;
		printf("\n          ********************\n");
		printf ("\nMessaggio numero: %d  | Mittente messaggio: %s | Oggetto messaggio: %s\n%s\n", input.id_messaggio, input.mittente, input.oggetto, input.messaggio);
		printf("          ********************\n");
		valore_ritorno = htons(1);
		sendThroughSocket(sock, sizeof(int), &valore_ritorno);
	}
	free(toReceive);
	return;
}
 
int main(int argc , char *argv[]){
	
	if (argc < 3){
	printf("\nUtilizzo: Client [opzioni]\n\nOpzioni:\n  indirizzo numero_porta                                 Efettua la connessione con il Server\n\n");
		return(-1);
		}
	
	int sock;
    struct sockaddr_in server;
	int scelta;
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    
    signal(SIGPIPE, SIG_IGN);
     
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("\nConnessione effettuata correttamente, benvenuto nel sistema!");
	while(1){
		printf("\n1 - Effettua l'accesso al sistema\n2 - Leggi tutti i messaggi presenti\n3 - Inserisci un nuovo messaggio\n4 - Elimina un messaggio\n5 - Termina esecuzione programma\n\nQuale operazione vuoi eseguire?\n");	
		scanf("%d", &scelta);
		fflush(stdin);
	
	switch (scelta) {
	
	case 1: // Login
		login(sock);
		break;
	
	case 2: // Leggi tutti i messaggi presenti
		see_all_messages(sock);
		break;
	
	case 3: // Inserimento nuovo messaggio
		insert_message(sock);
		break;
	
	case 4: // Elimina messaggio
		delete_message(sock);
		break;
				
	case 5: // Termina esecuzione programma
		close(sock);
		return 0;
        default:
                printf("\nInserisci un numero corretto per continuare!\n");
                break;
		}
	}
 }

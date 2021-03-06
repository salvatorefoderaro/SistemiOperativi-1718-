#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>    
#include <sys/socket.h>    
#include <arpa/inet.h> 
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define fflush(stdin) 
#define doppio_login "\n    *****    Il login è stato già effettuato!    *****\n\n"
#define errore_sessione "\n    *****    Per eseguire quest'operazione, effettua prima l'accesso!    *****\n\n"
#define flushTerminal printf("\nPremi invio per continuare..."); while(getchar() != '\n');  printf("\e[1;1H\e[2J");
#define flush_terminal_no_input printf("\e[1;1H\e[2J");
#define errore_login "\n    *****    Username o password errati!    *****\n\n"
#define errore_comunicazione "\n    *****    Errore nella comunicazione con il server!    *****\n\n"
#define errore_stringa_vuota "\n    *****    Non è consensita una stringa vuota per l'operazione!    *****\n\n"

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
    /* 
	Codifica i campi della struttura del tipo comunicazione,
	restituendo una stringa
	*/
	char *buffer = malloc(sizeof(struct comunicazione)+3*sizeof(char));
    sprintf(buffer, "%d|%d|%s|%s", htons(struttura->operazione), htons(struttura->valore_ritorno),struttura->argomento1, struttura->argomento2);
    return buffer;  
}

void decodeMessage(char *buffer, struct messaggi *struttura){
	/*
	Decodifica la stringa buffer, inserendo il risultato della tokenizzazione
	nei vari campi della struttura
	*/
   struttura->id_messaggio = ntohs(atoi(strtok(buffer, "|")));
   struttura->id_utente = ntohs(atoi(strtok(NULL, "|")));
   strcpy(struttura->messaggio, strtok(NULL, "|"));
   strcpy(struttura->mittente, strtok(NULL, "|"));
   strcpy(struttura->oggetto, strtok(NULL, "|"));
}
 
int sendThroughSocket(int socket, int size, void *buffer){
	// Invia, utilizzando il socket indicato, un numero di byte pari a size presenti in buffer
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
	// Riceve, utilizzando il socket indicato, un numero di byte pari a size scrivendoli in buffer
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

void registrazione(int sock){
		session.operazione = 4;
		printf("\nInserisci il nome utente\n");
		fgets(session.argomento1, 512, stdin); 
		printf("\nInserisci la password\n");
		fgets(session.argomento2, 512, stdin); 
		
		char *toSend = encode(&session);
		sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
		free(toSend);
		receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
		valore_ritorno = ntohs(valore_ritorno)  - 1000;
		
		if (valore_ritorno > 0){
			printf("\n    *****    Registrazione effettuata correttamente!    *****\n");
			flushTerminal
			return;
		}else if (valore_ritorno == -1){
			printf(doppio_login);
			flushTerminal	
			return;
		}else if (valore_ritorno == -11){
			printf("\n    *****    Stringa vuota non permessa!    *****\n");
			flushTerminal
			return;
			}else {
			printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
			flushTerminal
			return;
		}
}

void disconnect(int sock){
		session.operazione = 5;
		strcpy(session.argomento1, "NULL");
		strcpy(session.argomento2, "NULL");
		
		char *toSend = encode(&session);
		sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
		free(toSend);
		receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
		valore_ritorno = ntohs(valore_ritorno)  - 1000;
		
		if (valore_ritorno > 0){
			printf("\n    *****    Disconnessione effettuata correttamente!    *****\n");
			flushTerminal
			return;
		}else if (valore_ritorno == -2){
			printf(errore_sessione);
			flushTerminal
			return;
		} else {
			printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
			flushTerminal
			return;
		}
}

void login(int sock){

		session.operazione = 0;
		printf("\nInserisci il nome utente\n");
		fgets(session.argomento1, 512, stdin); 
		printf("\nInserisci la password\n");
		fgets(session.argomento2, 512, stdin); 
		// Codificio la password in stringa
		
		char *toSend = encode(&session);
		sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
		free(toSend);
		receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
		valore_ritorno = ntohs(valore_ritorno)  - 1000;
		if (valore_ritorno > 0){
			printf("\n\n    *****    Accesso effettuato correttamente!    *****\n\n");
			flushTerminal
			return;
		}else if (valore_ritorno == -1){
			printf(doppio_login);
			flushTerminal
			return;
		} else if (valore_ritorno == -3){
			printf(errore_login);
			flushTerminal
			return;
		} else if (valore_ritorno == -11){
			printf(errore_stringa_vuota);
			flushTerminal
			return;
		}else {
			printf("\n\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n\n", valore_ritorno);
			flushTerminal
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
		flushTerminal
		return;
	} else if (valore_ritorno == -11){
		printf(errore_stringa_vuota);
		flushTerminal
		return;
	} else if(valore_ritorno > 0){
		printf("\n    *****    Il messaggio è stato inserito correttamente!    *****\n");
		flushTerminal
		return;
	} else{
		printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
		flushTerminal
		return;
	}
}

void delete_message(int sock){
	session.operazione = 3;
	printf("\nQuale messaggio vuoi eliminare?\n");
	
	char messaggio_eliminare[32];
	fgets(messaggio_eliminare, 32, stdin); 
	int i_messaggio_eliminare = atoi(messaggio_eliminare);
	session.valore_ritorno = i_messaggio_eliminare;
	
	strcpy(session.argomento1, "NULL");
	strcpy(session.argomento2, "NULL");
	
	char *toSend = encode(&session);
	sendThroughSocket(sock, sizeof(struct comunicazione)+3*sizeof(char), toSend);
	free(toSend);
	receiveThroughSocket(sock, sizeof(int), &valore_ritorno);
	valore_ritorno = ntohs(valore_ritorno) - 1000;
	
	if (valore_ritorno > 0){
		printf("\n    *****    Messaggio eliminato correttamente!    *****\n");
		flushTerminal
		return;
	} else if (valore_ritorno == -2){
		printf(errore_sessione);
		flushTerminal
		return;
	} else if (valore_ritorno == -6){
		printf("\n    *****    Nessun messaggio presente!    *****\n");
		flushTerminal
		return;
	} else if (valore_ritorno == -8){
		printf("\n    *****    Il messaggio che vuoi eliminare non è presente!    *****\n");
		flushTerminal
		return;
	} else if (valore_ritorno == -9){
		printf("\n    *****    Non hai il permesso per eliminare il messaggio!    *****\n");
		flushTerminal
		return;
	} else{
		printf("\n    *****    Errore nella richiesta. Codice di errore: %d    *****\n", valore_ritorno);
		flushTerminal
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
		flushTerminal
		return;
	}
	char *toReceive = malloc(sizeof(struct messaggi)+4*sizeof(char));
        printf("\n          ********************\n");
	while(dimensione > 0){
		int dataReceived  = receiveThroughSocket(sock, sizeof(struct messaggi)+4*sizeof(char), toReceive);
		decodeMessage(toReceive, &input);
		dimensione = dimensione - dataReceived;
		printf ("\nMessaggio numero: %d  | Mittente messaggio: %s | Oggetto messaggio: %s\n%s\n", input.id_messaggio, input.mittente, input.oggetto, input.messaggio);
		printf("          ********************\n");
		valore_ritorno = htons(1);
		sendThroughSocket(sock, sizeof(int), &valore_ritorno);
	}
	flushTerminal
	free(toReceive);
	return;
}
 
int main(int argc , char *argv[]){

	if (argc < 3 || argc > 4){
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
        perror("Errore nella creazione del Socket.");
		return -1;
    }
    
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	if ((sigemptyset(&act.sa_mask) == -1) || sigaction(SIGPIPE, &act, NULL)) {
		printf("\nErrore nell'inizializzazione del gestore dei segnali...\n");
	}
     
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Connessione fallita.");
        return 1;
    }
	flush_terminal_no_input
    printf("\nConnessione effettuata correttamente, benvenuto nel sistema!\n");
	char scelta_utente[32];
	while(1){
		printf("\n1 - Effettua l'accesso al sistema\n2 - Leggi tutti i messaggi presenti\n3 - Inserisci un nuovo messaggio\n4 - Elimina un messaggio\n5 - Effettua la registrazione\n6 - Effettua disconnessione\n7 - Termina esecuzione programma\n\nQuale operazione vuoi eseguire?\n");	
		fgets(scelta_utente, 32, stdin);
	scelta = atoi(scelta_utente);
	switch (scelta) {
	
	case 1: // Login
	flush_terminal_no_input
		login(sock);
		break;
	
	case 2: // Leggi tutti i messaggi presenti
		flush_terminal_no_input
		see_all_messages(sock);
		break;
	
	case 3: // Inserimento nuovo messaggio
		flush_terminal_no_input
		insert_message(sock);
		break;
	
	case 4: // Elimina messaggio
		flush_terminal_no_input
		delete_message(sock);
		break;
	
	case 5: // Registrazione
		flush_terminal_no_input
		registrazione(sock);
		break;
	
	case 6: // Disconnessione
		flush_terminal_no_input
		disconnect(sock);
		break;
	
	case 7: // Termina esecuzione programma
		flush_terminal_no_input
		close(sock);
		return 0;
        default:
				flush_terminal_no_input
                printf("\nInserisci un numero corretto per continuare!\n");
				flushTerminal
                break;
		}
	}
 }
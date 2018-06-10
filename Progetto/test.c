#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#define fflush(stdin) while(getchar() != '\n')
#define file_utenti "user.dat"
#define file_messaggi "person.dat"
#define file_consistenza "consistenz.dat"
#define utente_non_loggato 		printf("\nOperazione non consentita! Devi effettuare prima l'accesso!\n\nPremi un tasto per continuare...");getchar(); 
#define messaggio_non_loggato "\nOperazione non consentita! Devi effettuare prima l'accesso!\n"
 
struct consistenza_informazioni {
	int ultimo_id_messaggio;
	int ultimo_id_utente;
	};

struct sessione {
	int id_utente_loggato;
	char nome_utente_loggato[64];
	};

struct messaggi {
	int id_utente;
	int id_messaggio;
	char messaggio[512];
	char oggetto[128];
	char mittente[64];
	};

struct utente {
	int id_utente;
	char nome_utente[64];
	char password_utente[64];
	};
	
struct consistenza_informazioni consistenza_sessione;
__thread struct sessione utente_loggato;
sem_t semaforo;

void *recupero_consistenza_informazioni(){
	
	FILE *infile;
    struct messaggi recupero;
    
    infile = fopen (file_messaggi, "r");
    if (infile == NULL)
    {
		consistenza_sessione.ultimo_id_messaggio = 0;
		return(-1);
    }

    fseek(infile, -(sizeof(struct messaggi)), SEEK_END);
	fread(&recupero, sizeof(struct messaggi), 1, infile);
    consistenza_sessione.ultimo_id_messaggio = recupero.id_messaggio;
    fclose(infile);
    
    return 0;
	
	}

void visualizza_utenti(){
	
	FILE *infile;
    struct utente input;
    
    infile = fopen (file_utenti, "r");
    if (infile == NULL)
    {
        printf("\nAl momento non è presente nessun utente!\n");
		return(-1);
    }
    printf("\nID Utente | Nome utente | Password utente\n");
    while(fread(&input, sizeof(struct utente), 1, infile)){
        printf ("\n%d | %s | %s\n", input.id_utente, input.nome_utente, input.password_utente);
    }
    fclose(infile);
    return 0;
	
	}

void *gestore_utente(){
	
	utente_loggato.id_utente_loggato = 0;
	
	char messaggio[512];
	char oggetto[128];
	char mittente[64];
	char nome_utente[64];
	char password_utente[64];
	int scelta;
	int valore_vario;
	int valore_ritorno = 200;

	while(1){
	printf("\nIl valore di consistenza attuale è: %d\n", consistenza_sessione.ultimo_id_messaggio);
	printf("\nQuale operazione vuoi eseguire?\n0 - Inserisci un nuovo messaggio\n1 - Leggi tutti i messaggi presenti\n2 - Effettua l'accesso al sistema\n3 - Elimina un messaggio\n4 - Termina l'esecuzione del programma\n\nInserisci il numero del comando che vuoi eseguire: ");
		
	scanf("%d", &scelta);
	while(getchar() != '\n');
	switch (scelta) {
	case 0:
	
	if(utente_loggato.id_utente_loggato != 0){
		printf("Quale messaggio vuoi inserire?\n");
		gets(messaggio);
		fflush(stdin);
		printf("Quale oggetto vuoi inserire?\n");
		gets(oggetto);
		fflush(stdin);
		inserisci_nuovo_messaggio(messaggio, oggetto, utente_loggato.nome_utente_loggato, utente_loggato.id_utente_loggato);
		break;
	}
	else
	{
		utente_non_loggato
		break;
	}
	
	case 1:
	
	leggi_tutti_messaggi();
	break;
	
	case 2:
	
	printf("\nInserisci il nome utente\n");
	scanf("%s", nome_utente);
	printf("Inserisci la password\n");
	scanf("%s", password_utente);
	utente_loggato.id_utente_loggato = controllo_accesso(nome_utente, password_utente);
	if(utente_loggato.id_utente_loggato > 0){
		strcpy(utente_loggato.nome_utente_loggato, nome_utente);
		break;
	}
	else
	{
		printf("\nErrore nella procedura di log-in!\n");
		break;
	};
	case 3:
		if(utente_loggato.id_utente_loggato != 0){
			printf("\nQuale messaggio vuoi eliminare?\n");
			scanf("%d", &valore_vario);
			fflush(stdin);
			elimina_messaggio(utente_loggato.id_utente_loggato, valore_vario);
			break;
		}
		else{
		utente_non_loggato
			break;
		}
	case 4:
		pthread_exit(&valore_ritorno);
	}
	}
	}
 
int elimina_messaggio(int id_utente, int id_messaggio){
	
	if(utente_loggato.id_utente_loggato == 0){
		utente_non_loggato
			return -1;
		}
	FILE *leggo;
	FILE *scrivo;
    struct messaggi controllo;
  
    leggo = fopen (file_messaggi, "r");
    if (leggo == NULL)
    {
        fprintf(stderr, "\nErrore nell'apertura del file!\n");
        return -1;
    }
    
	scrivo = fopen ("passaggio.dat", "w+");
    if (scrivo == NULL)
    {
        fprintf(stderr, "\nErrore nell'apertura del file!\n");
        return -1;
    }
    while(fread(&controllo, sizeof(struct messaggi), 1, leggo)){
		if(controllo.id_utente == id_utente & controllo.id_messaggio == id_messaggio){
			continue;
		}
		else
		{
			fwrite(&controllo, sizeof(struct messaggi), 1, scrivo);
			}
			}
        
    fclose(leggo);
    unlink("person.dat");
    link("passaggio.dat", "person.dat");
    fclose(scrivo);
	unlink("passaggio.dat");
    return 0;
	}

int controllo_accesso(char *nome_utente, char *password){
	
	if(utente_loggato.id_utente_loggato != 0){
		printf("Un utente è già collegato tramite questo thread!");
		return -1;
		}

	FILE *infile;
    struct utente input;
     
    infile = fopen ("user.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nErrore nell'apertura del file!\n");
        return -1;
    }
     
    while(fread(&input, sizeof(struct utente), 1, infile)){
		if(strcmp(&(input.nome_utente),nome_utente) == 0 & strcmp(&(input.password_utente), password) == 0){
				printf("\nAccesso effettuato correttamente. Benvenuto %s!\n", input.nome_utente); // Uscire dal while quando ho trovato una corrispndenza
				return input.id_utente;
			}
        }
    
    fclose(infile);
    return 0;
	}

int inserisci_nuovo_messaggio(char *messaggio, char *oggetto, char *mittente, int id_utente_loggato){
	
	if(utente_loggato.id_utente_loggato == 0){
		utente_non_loggato
		return -1;
		}
	
	printf("Il codice dell'utente che vado ad inserire è: %d", id_utente_loggato);
	// Inserisco il nuovo messaggio
	struct messaggi nuovo_messaggio;
	nuovo_messaggio.id_utente = id_utente_loggato;
	nuovo_messaggio.id_messaggio = (consistenza_sessione.ultimo_id_messaggio) + 1;
	strcpy(nuovo_messaggio.messaggio, messaggio);
	strcpy(nuovo_messaggio.oggetto, oggetto);
	strcpy(nuovo_messaggio.mittente, mittente);
	
	FILE *outfile;
     
    outfile = fopen ("person.dat", "a+");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nErrore nell'apertura del file\n");
        return -1;
    }
     
    if (fwrite(&nuovo_messaggio, sizeof(struct messaggi), 1, outfile) < 0){
		fprintf(stderr, "\nErrore nella scrittura su file!\n");
		}  
    fclose(outfile);

	} 
 
 int inserisci_nuovo_utente(int id_utente, char *nome_utente, char *password_utente){
	
	// Inserisco il nuovo messaggio
	struct utente nuovo_utente;
	
	nuovo_utente.id_utente = id_utente;
	strcpy(nuovo_utente.nome_utente, nome_utente);
	strcpy(nuovo_utente.password_utente, password_utente);
	
	FILE *outfile;
     
    outfile = fopen ("user.dat", "a+");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
     
    fwrite (&nuovo_utente, sizeof(struct utente), 1, outfile);  
    fclose(outfile);
	}
 
int leggi_tutti_messaggi (void){

    FILE *infile;
    struct messaggi input;
    infile = fopen ("person.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nErrore nell'apertura del file\n");
		return(-1);
    }
    fseek(infile, 0, SEEK_END);
    if (ftell(infile) < sizeof(struct messaggi)){
		printf("\nNessun messaggio presente!\nPremi un tasto per continuare...\n");
		getchar();
		return(-1);
	}
	fseek(infile, 0, SEEK_SET);
    while(fread(&input, sizeof(struct messaggi), 1, infile)){
        printf ("\nMessaggio numero: %d  | Mittente messaggio: %s | Oggetto messaggio: %s\n\n%s\n", input.id_messaggio, input.mittente, input.oggetto, input.messaggio);
    }
    fclose(infile);
    return 0;
	} 
 
int main (int argc, char **argv){
	if (argc > 1){
	if (strcmp(argv[1], "-seeuser") == 0){
		visualizza_utenti();
		return 0;
		}
	
	if(strcmp(argv[1], "-insertuser") == 0){
		inserisci_nuovo_utente(atoi(argv[2]), argv[3], argv[4]);
		return 0;
		}
	}
	
	
	pthread_t thread1, thread2;
	pthread_create(&thread2, NULL, recupero_consistenza_informazioni, NULL);
	pthread_join(thread2, NULL);
	
	pthread_create(&thread1, NULL, gestore_utente, NULL);
	pthread_join(thread1, NULL);
	return 0;
	
}

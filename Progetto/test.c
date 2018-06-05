// C program for writing 
// struct to file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
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
	int messaggi_utente[128];
	};
	
int accesso_utente = 0;

int controllo_accesso(char *nome_utente, char *password){
	
	FILE *infile;
    struct utente input;
     
    infile = fopen ("user.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
     
    while(fread(&input, sizeof(struct utente), 1, infile)){
		if(strcmp(&(input.nome_utente),nome_utente) == 0 & strcmp(&(input.password_utente), password) == 0){
				printf("Accesso effettuato correttamente. Benvenuto %s!", input.nome_utente); // Uscire dal while quando ho trovato una corrispndenza
				accesso_utente = input.id_utente; // Sempre uscire dal While quando trovo corrispondenza
			}
        }
    fclose(infile);
    return 0;
	
	}

int inserisci_nuovo_messaggio(char *messaggio, char *oggetto, char *mittente){
	
	// Inserisco il nuovo messaggio
	struct messaggi nuovo_messaggio;
	nuovo_messaggio.id_utente = 100;
	nuovo_messaggio.id_messaggio = 1000;
	strcpy(nuovo_messaggio.messaggio, messaggio);
	strcpy(nuovo_messaggio.oggetto, oggetto);
	strcpy(nuovo_messaggio.mittente, mittente);
	
	FILE *outfile;
     
    outfile = fopen ("person.dat", "a+");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
     
    fwrite (&nuovo_messaggio, sizeof(struct messaggi), 1, outfile);
    
		// Controllo se il messaggio è stato scritto correttamente. 
		// In caso positivo, aggiungo il messaggio alla lista dei messaggi del singolo utente
		// Come per la modifica, devo riposizionare il puntatore in quanto devo aggiungere le nuove modifiche
    
    fclose(outfile);
	} 
 
int leggi_tutti_messaggi (void){
	
    FILE *infile;
    struct messaggi input;
    infile = fopen ("person.dat", "r");
    if (infile == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
     
    while(fread(&input, sizeof(struct messaggi), 1, infile)){
        printf ("Messaggio numero: %d  | Mittente messaggio: %s. Oggetto messaggio: %s\n\n%s\n\n", input.id_messaggio, input.mittente, input.oggetto, input.messaggio);
    }
    fclose(infile);
    return 0;
	} 
 
void main ()
{

    FILE *outfile;
     
    outfile = fopen ("user.dat", "a+");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
	struct utente nuovo_messaggio = {0,"Salvatore", "Foderaro95", NULL};
	
    //struct messaggi nuovo_messaggio = {100, 1, "rohan", "sharma", "Weee"};
    //inserisci_nuovo_messaggio("AAAAAAa", "EEEEEEEE", "CCCCCCCC");
    controllo_accesso("Salvatore", "Foderaro95");
    	printf("Dimensione è: %d", sizeof(struct utente));

     
    fwrite (&nuovo_messaggio, sizeof(struct messaggi), 1, outfile);
    printf("Seek è: %d", fseek(outfile, 0, SEEK_CUR));
    printf("Weee: %ld", ftell(outfile));

    fclose(outfile);
        printf("contents to file written successfully!\n");
        // leggi_tutti_messaggi();
}

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




int inserisci_nuovo_messaggio(char *messaggio, char *oggetto, char *mittente){
	
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
    fclose(outfile);
    printf("1 | contents to file written successfully!\n");
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
     
    outfile = fopen ("person.dat", "a+");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }

    struct messaggi nuovo_messaggio = {100, 1, "rohan", "sharma", "Weee"};
    //inserisci_nuovo_messaggio("AAAAAAa", "EEEEEEEE", "CCCCCCCC");
    	printf("Dimensione è: %d", sizeof(struct messaggi));

     
    fwrite (&nuovo_messaggio, sizeof(struct messaggi), 1, outfile);
    printf("Seek è: %d", fseek(outfile, 0, SEEK_CUR));
    printf("Weee: %ld", ftell(outfile));

    fclose(outfile);
        printf("contents to file written successfully!\n");
        // leggi_tutti_messaggi();
}

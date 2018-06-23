#include <stdio.h>
#include <stdlib.h>

int main(void){

	// Il nome del puntatore rappresenta sempre l'indirizzo dell'oggetto base che mi rappresenta da utilizzare per la Malloc, come in questo caso.
	//
	// Il singolo deferenziamento mi permette di accedere all'indirizzo dell'elemento, mentre il doppio all'elemento stesso.

	char **lista;
	lista = malloc(10); // Devo fare il malloc altrimenti ho errore di segmentazione, in quanto il puntatore dichiarato in quel modo è 'virtuale'

	char *parola = "Questaaaa";
	char *parola1 = "Altraaa";
	
	*lista= parola; // Indirizzo dello 0-esimo elemento
	*(lista+1) = parola1; // Indirizzo del primo elemento


	// ----- Altro modo -----
	//
	
	char *lista1[10];
	lista[0] = parola;
	lista[1] = parola1;
	printf("%s %s", lista[0], lista[1]);
}

#include <stdio.h>
#include <stdlib.h>
#define fflush(stdin) while(getchar() != '\n')
	struct lista {
		int valore;
		struct lista *next;
	};

void stampa_valori(struct lista **primo_elemento){

	// Scorro la lista e stampo tutti i valori presenti

	struct lista *supporto = (*primo_elemento);
	printf("Il valore è: %d\n", supporto->valore);
	while(supporto->next != NULL){
		printf("Il valore è: %d\n", supporto->next->valore);
		supporto = supporto->next;
	}

	return;

}

int inserimento_ordinato(struct lista **primo_elemento, int valore_inserire){
	
	// Creo una struttura di supporto

	struct lista *nuovo_elemento = malloc(sizeof(struct lista));
	nuovo_elemento->valore = valore_inserire;
	nuovo_elemento->next = NULL;
	
	// Controllo se il nuovo elemento va inserito primo in lista

	if (valore_inserire <= (*primo_elemento)->valore){
		nuovo_elemento->next = *primo_elemento;
		*primo_elemento = nuovo_elemento;
		return 1;
	}

	// Controllo se il nuovo elemento va inserito tra due elementi	

	else{
		struct lista *supporto = *primo_elemento;
		while(supporto->next != NULL){
				if (valore_inserire > supporto->valore && valore_inserire <= supporto->next->valore){
				nuovo_elemento->next = supporto->next;
				supporto->next = nuovo_elemento;
				return 1;
			}
		// Controllo l'elemento successivo
		supporto = supporto->next;
		}

	// Altrimenti lo inserisco al termine della lista
	supporto->next = nuovo_elemento;
	}

	return;
}


int main(void){
	int n;
	struct lista **primo_elemento = malloc(sizeof(struct lista));
	struct lista *lista_inserimento = malloc(sizeof(struct lista));
	lista_inserimento->valore = 10;
	lista_inserimento->next = NULL;
	*primo_elemento = lista_inserimento;
	printf("Hello world!\n");
	while(n != -1){
	scanf("%d", &n);
	fflush(stdin);
	inserimento_ordinato(primo_elemento, n);
	}
	stampa_valori(primo_elemento);
}

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
	
	int niceness; int priorita = 0; int policy; pid_t processID;
	uid_t uid;

	uid = getuid();
	processID = getpid();

	struct sched_param {
		int sched_priority;
	};

	struct sched_param a;
	while(priorita != 100){
	printf("\nCiao! Lo scheduler attuale è: %d. Quale proprità vuoi impostare per il processo? (Devi avere i permessi di Root per impostare come priorità un valore compreso tra (1 e 99). Altrimenti scrivi (100) per terminare.\n",sched_getscheduler(processID));
		scanf("%d", &priorita);
		if(priorita == 100){goto label;}
		a.sched_priority = priorita;
		while((getchar()) != '\n');
		printf("Quale niceness vuoi impostare invece (-20 a 19)?\n");
		scanf("%d", &niceness);
		while((getchar()) != '\n');
	printf("Quale policy voui impostare?\nSCHED_OTHER (0), SCHED_FIFO(1), SCHED_RR(2)\n");
		scanf("%d", &policy);
		while((getchar()) != '\n');
	if(sched_setscheduler(processID, policy, &a) == 0){ // SCHED_FIFO si può modificare. Vedi man per le possibili scelte
		if (nice(niceness) != 0){
			printf("Lo scheduler e la niceness sono stati impostati correttamente!\n");
			}
		else{
			printf("Lo scheduler è stato impostato correttamente, mentre c'è stato un errore per quanto riguarda la niceness\n");
		}
	}
	else if (uid != 0 & priorita > 0){
		printf("Sono necessari i permessi di Root per eseguire questa operazione!\n");
	}
	else if (policy == 0 & priorita > 0) {
		printf("Non è possibile impostare una proprità maggiore di 0 per un processo con policy SCHED_OTHER (0)\n");
	}
	else {

	printf("Errore da qualche parte!");

	}
	}
	return 0;
label:
	while(1){
	
		
	
	};

}


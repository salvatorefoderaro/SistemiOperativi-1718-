// TestSistemiOperativi.cpp: definisce il punto di ingresso dell'applicazione console.
//

// ConsoleApplication1.cpp: definisce il punto di ingresso dell'applicazione console.
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void PrioritaThread(VOID *valoreNumerico) {
	HANDLE maniglia; 	DWORD priorita, prioritaThread;
	int *valore;
	valore = (int *)valoreNumerico; // Cioè valore, che è l'indirizzo del nuovo puntatore, deve essere uguale all'indirizzo del puntatore VOID castato come intero però.

	maniglia = GetCurrentProcess();
	priorita = GetPriorityClass(maniglia);
	 
	// Essendo un puntatore di tipo VOID, cioè che accetta qualsiasi tipo di dato, devo castarlo correttamente.
	// Cioè devo indicare a quale tipo di dato corrisponde. Quindi devo castarlo come un puntatore al tipo di dato che mi interessa.

	printf("\nIl parametro passato al Thread è: %d\n", *valore);
	prioritaThread = GetThreadPriority(GetCurrentThread());

	printf("La classe di proprità attuale e': 0x%x | Mentre il valore della priorità e': %d\n", priorita, prioritaThread);
	if (SetThreadPriority(GetCurrentThread(), *valore) != 0) {
		printf("Priorità modificata con successo! La nuova priorità è: %ld\n", GetThreadPriority(GetCurrentThread()));
	}
	else {
		printf("Errore, non è stato possibile modificare la proprità del thread! Sicuro di aver immesso un intero valido?\n");
	}
	Sleep(5000);

}

void modificaMascheraProcesso() {
	HANDLE maniglia;

	maniglia = GetCurrentProcess();

	int scelta = 100; DWORD e; DWORD f;
	printf("Ciao! Cosa vuoi fare?\nOttieni la maschera attuale (0) (la maschera verrà stampata in formato esadecimale), imposta una nuova maschera per il processo in esecuzione (1) oppure digita (2) per terminare!\n");
		
	while (scelta != 2) {
			printf("Cosa vuoi fare?\n");
			scanf("%d", &scelta);
			while ((getchar()) != '\n'); // Pulisco il buffer di input

			if (scelta == 0) {
				GetProcessAffinityMask(maniglia, &e, &f);
				printf("Maschera sistema: %d - Maschera processo: %d\n", e, f);
			}

			if (scelta == 1) {
				int nuovo_valore;
				printf("Inserisci la nuova maschera che vuoi assegnare al processo!\n");
				scanf("%d", &nuovo_valore);
				if (nuovo_valore == 0) {
					printf("Errore! Non è possibile assegnare una maschera vuota al processo!\n");
				}
				if (!SetProcessAffinityMask(maniglia, (DWORD_PTR)nuovo_valore)) {
					printf("Errore! La maschera non è stata modificata\n");
				}
				else {
					printf("Maschera modificata correttamente\n");
				}
			}
		}
		ExitThread(200);
}

void modificaMascheraThread() {
	HANDLE maniglia;

	maniglia = GetCurrentThread();

	int scelta = 100; DWORD e; DWORD f;
	printf("Ciao! Cosa vuoi fare?\nOttieni la maschera attuale (0) (la maschera verrà stampata in formato esadecimale), imposta una nuova maschera per il processo in esecuzione (1) oppure digita (2) per terminare!\n");
		
	while (scelta != 2) {
			printf("Cosa vuoi fare?\n");
			scanf("%d", &scelta);
			while ((getchar()) != '\n'); // Pulisco il buffer di input

			if (scelta == 0) {
				GetThreadAffinityMask(maniglia, &e, &f);
				printf("Maschera sistema: %d - Maschera processo: %d\n", e, f);
			}

			if (scelta == 1) {
				int nuovo_valore;
				printf("Inserisci la nuova maschera che vuoi assegnare al processo!\n");
				scanf("%d", &nuovo_valore);
				if (nuovo_valore == 0) {
					printf("Errore! Non è possibile assegnare una maschera vuota al processo!\n");
				}
				if (!SetThreadAffinityMask(maniglia, (DWORD_PTR)nuovo_valore)) {
					printf("Errore! La maschera non è stata modificata\n");
				}
				else {
					printf("Maschera modificata correttamente\n");
				}
			}
		}
		ExitThread(200);
}


int main(void)
{
	DWORD threadId; HANDLE thread; HANDLE thread1; DWORD threadId1;

	/*		TUTTA ROBA SUI PROCESSI, AL MOMENTO LAVORO CON I THREAD QUINDI NON SERVE

	BOOL newprocess;
	STARTUPINFO si; PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si)); memset(&pi, 0, sizeof(pi)); si.cb = sizeof(si);
	newprocess = CreateProcess("C:/Windows/System32/cmd.exe", (LPSTR)"ipconfig", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	newprocess = CreateProcess(".\\a.exe", (LPSTR)".\\a.exe", NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	
	*/
	
	DWORD we = 2;
	thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)modificaMascheraProcesso, NULL, NORMAL_PRIORITY_CLASS, &threadId);
	WaitForSingleObject(thread, INFINITE);
	thread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PrioritaThread, (LPVOID)&we, NORMAL_PRIORITY_CLASS, &threadId1);
	WaitForSingleObject(thread1, INFINITE);

}


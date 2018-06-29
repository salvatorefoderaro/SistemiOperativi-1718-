/******************************************************************
Welcome to the Operating System examination

You are editing the '/home/esame/prog.c' file. You cannot remove
this file, just edit it so as to produce your own program according to
the specification listed below.

In the '/home/esame/'directory you can find a Makefile that you can
use to compile this program to generate an executable named 'prog'
in the same directory. Typing 'make posix' you will compile for
Posix, while typing 'make winapi' you will compile for WinAPI just
depending on the specific technology you selected to implement the
given specification. Most of the requested header files (for either
Posix or WinAPI compilation) are already included at the head of the
prog.c file you are editing.

At the end of the examination, the last saved snapshot of this file
will be automatically stored by the system and will be then considered
for the evaluation of your exam. Moifications made to prog.c which you
did not saved via the editor will not appear in the stored version
of the prog.c file.
In other words, unsaved changes will not be tracked, so please save
this file when you think you have finished software development.
You can also modify the Makefile if requesed, since this file will also
be automatically stored together with your program and will be part
of the final data to be evaluated for your exam.

PLEASE BE CAREFUL THAT THE LAST SAVED VERSION OF THE prog.c FILE (and of
the Makfile) WILL BE AUTOMATICALLY STORED WHEN YOU CLOSE YOUR EXAMINATION
VIA THE CLOSURE CODE YOU RECEIVED, OR WHEN THE TIME YOU HAVE BEEN GRANTED
TO DEVELOP YOUR PROGRAM EXPIRES.


SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[1] un numero
intero N maggiore o uguale ad 1 (espresso come una stringa di cifre
decimali), e generi N nuovi thread. Ciascuno di questi, a turno, dovra'
inserire in una propria lista (in testa o in coda) basata su memoria
dinamica un record strutturato come segue:

typedef struct _data{
	int val;
	struct _data* next;
} data;

I record vengono generati e popolati dal main thread, il quale rimane
in attesa indefinita di valori interi da standard-input. Ad ogni nuovo
valore letto avverra' la generazione di un nuovo record, che verra'
inserito da uno degli N thread nella sua lista.
L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra'
stampare a terminale il contenuto corrente di tutte le liste (ovvero
i valori interi presenti nei record correntemente registrati nelle liste
di tutti gli N thread).
Nel caso in cui non vi sia immissione di input, l'applicazione non dovra'
consumare piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/
#ifdef Posix_compile
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char** argv)
{

    return 0;
}
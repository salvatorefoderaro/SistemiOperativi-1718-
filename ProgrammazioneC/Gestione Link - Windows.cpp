// Gestione Link - Windows.cpp: definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#ifdef _UNICODE
typedef wchar_t TCHAR;
#else
typedef char TCHAR;
#endif

int main()
{	

	TCHAR fileOriginale[200];
	TCHAR fileCopia[200];
	scanf(TEXT("%s"), &fileOriginale);
	scanf(TEXT("%s"), &fileCopia);

	CreateHardLink((fileCopia), (fileOriginale), NULL); // Copia file

	// CreateSymbolicLink((fileCopia), (fileOriginale), 0); // Collegamento file
	
	printf("Errore numero: %d", GetLastError());

	Sleep(10000);
}


#include "../malloc/malloc_stub.h"
#include "syscall.h"
#define SIZE 20

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testMalloc : \n");
	PutString("Demande une chaine a l'utilisateur qu'il affiche ensuite\nvia un buffer alloue au debut.\n");
	PutString("-----------------------------------------\n");

	PutString("Entrer une chaine de caracteres :\n");
	char* buffer = (char*)malloc(SIZE * sizeof(char));
	if(buffer == 0)
	{
		PutString("Erreur de malloc.\n");
	}
	else
	{
		GetString(buffer, SIZE);
		PutString(buffer);
	}
	free(buffer);
	return 0;
}

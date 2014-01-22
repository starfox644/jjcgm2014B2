#include "syscall.h"

/*
 * Teste la creation de thread avec une mauvaise adresse d'argument
 * Ce test est cense afficher "Erreur de creation de thread."
 */


void f(void *a)
{
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadBadArg : \n");
	PutString("Lance un thread avec une mauvaise adresse d'argument.\n");
	PutString("Doit afficher \"Erreur de creation de thread\".\n");
	PutString("-----------------------------------------\n");
	if(UserThreadCreate(f, (void*)-1) == -1)
	{
		PutString("Erreur de creation de thread.\n");
	}
	else
	{
		PutString("Creation reussie.\n");
	}
    return 0;
}

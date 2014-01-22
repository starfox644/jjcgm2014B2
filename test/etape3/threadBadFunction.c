#include "syscall.h"

/*
 * Teste la creation de thread avec une mauvaise adresse de fonction.
 * Ce test est cense afficher "Erreur de creation de thread."
 */
int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadBadFunction : \n");
	PutString("Lance un thread avec une mauvaise adresse de fonction.\n");
	PutString("Doit afficher \"Erreur de creation de thread\".\n");
	PutString("-----------------------------------------\n");
	int a = 0;
	if(UserThreadCreate((void(*)(void*))500000, &a) == -1)
	{
		PutString("Erreur de creation de thread.\n");
	}
	else
	{
		PutString("Creation reussie.\n");
	}
    return 0;
}

#include "syscall.h"
/*
 * Teste la creation de thread avec une grande adresse de fonction.
 * La pile est alors placee en dehors de la memoire du MIPS, ce qui provoque une erreur.
 * Ce test est cense afficher "Erreur de creation de thread."
 */
int main()
{
	int a = 0;
	if(UserThreadCreate((void(*)(void*))32700, &a) == -1)
	{
		PutString("Erreur de creation de thread.\n");
	}
	else
	{
		PutString("Creation reussie.\n");
	}
    return 0;
}

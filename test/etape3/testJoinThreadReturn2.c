#include "syscall.h"

/**
 *	Le thread main cree un thread et attend sa terminaison. Il passe
 *	0 comme adresse ou stocker la valeur de retour de f ce qui a pour
 *	but de ne pas la recuperer. On le verifie en affichant ce qu'il y
 *	a a l'adresse 0. (on voit qu'elle n'est pas modifiee : ok)
 */

void f(void* arg)
{
	int i;
	if(PutString("\nthread f\n") == -1)
		PutString("Erreur putstring\n");
	for (i = 0 ; i < 1000 ; i++);
	PutString("f fini\n");
	UserThreadExit(42);
}

int main()
{
	int tidf;
	int* ad0 = (int*)0;
	tidf = UserThreadCreate(f, 0);
	PutString("Adr 0 avant join : ");
	PutInt(*ad0);
	PutString("\n");
	if(UserThreadJoin(tidf, 0) == -1)
		PutString("Erreur join du main sur f\n");

	PutString("\nAdr 0 apres join : ");
	PutInt(*ad0);
	PutString("\n");
	PutString("Fin du main\n");
    return 0;
}

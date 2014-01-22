#include "syscall.h"

/**
 * 	Premier test de thread join. Cree un thread qui effectue un affichage et une boucle.
 * 	Le thread principal attend ce thread et se termine.
 */

void f(void* arg)
{
	int i = 0;
	if(PutString("thread f\n") == -1)
		PutString("Erreur putstring\n");
	for(i = 0 ; i < 1000 ; i++);
	if(PutString("thread f fini\n") == -1)
			PutString("Erreur putstring\n");
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testJoin : \n");
	PutString("Cree un thread qui affiche une chaine en attendant qu'il se termine\n");
	PutString("-----------------------------------------\n");
	int tidf;
	tidf = UserThreadCreate(f, 0);
	if(UserThreadJoin(tidf, 0) == -1)
		PutString("Erreur join\n");
	PutString("Fin du main\n");
    return 0;
}

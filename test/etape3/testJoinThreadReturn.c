#include "syscall.h"

/**
 *	Le thread main cree un thread, recupere sa valeur de retour a l'aide
 *	de la fonction join puis l'affiche
 */

void f(void* arg)
{
	if(PutString("thread f\n") == -1)
		PutString("Erreur putstring\n");
	PutString("Thread f fini\n");
	UserThreadExit(42);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadMulJoin : \n");
	PutString("Cree un thread et recupere sa valeur de retour avec Join puis l'affiche.\n");
	PutString("-----------------------------------------\n");
	int tidf;
	int ret;
	tidf = UserThreadCreate(f, 0);
	if(UserThreadJoin(tidf, &ret) == -1)
		PutString("Erreur join du main sur f\n");
	else
	{
		PutString("Code retour f :  ");
		PutInt(ret);
		PutString("\n");
	}
	PutString("Fin du main\n");
    return 0;
}

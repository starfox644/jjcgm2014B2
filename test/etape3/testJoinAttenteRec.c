#include "syscall.h"

/**
 *	Le thread main cree 3 threads : h, g, f. Le main attend f, qui attend g,
 *	qui attend lui meme h.
 */
void h(void* arg)
{
	int i;
	if(PutString("thread h\n") == -1)
		PutString("Erreur putstring\n");
	for (i = 0 ; i < 500000 ; i++)
	{
	}
	PutString("Thread h fini\n");
}


void g(void* arg)
{
	int i;
	int* tidh = (int*)arg;
	if(PutString("thread g\n") == -1)
		PutString("Erreur putstring\n");
	for (i = 0 ; i < 50000 ; i++)
	{
	}
	if(UserThreadJoin(*tidh, 0) == -1)
		PutString("Erreur join de g sur h\n");
	PutString("Thread g fini\n");
}

void f(void* arg)
{
	int* tidg = (int*)arg;
	if(PutString("thread f\n") == -1)
		PutString("Erreur putstring\n");
	if(UserThreadJoin(*tidg, 0) == -1)
		PutString("Erreur join de f sur g\n");
	PutString("Thread f fini\n");
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testJoinAttenteRec : \n");
	PutString("Lance 3 threads f, g et h. Le main attend f qui attend g qui attend h.\n");
	PutString("-----------------------------------------\n");
	int tidf, tidg, tidh;
	tidh = UserThreadCreate(h, 0);
	tidg = UserThreadCreate(g, &tidh);
	tidf = UserThreadCreate(f, &tidg);

	if(UserThreadJoin(tidf, 0) == -1)
		PutString("Erreur join du main sur f\n");
	PutString("Fin du main\n");
    return 0;
}

#include "syscall.h"

/**
 * Le thread main cree deux threads : f et g. Les threads main et f attente tous les deux
 * g. On obtient une erreur sur un join : un seul thread peut attendre un thread donne
 */

void g(void* arg)
{
	int i;
	PutString("Thread g\n");
	for (i = 0 ; i < 500000 ; i++)
	{
	}
	PutString("g fini\n");
}

void f(void* arg)
{
	int* tidg = (int*)arg;
	PutString("Thread f\n");
	if(UserThreadJoin(*tidg) == -1)
		PutString("Erreur join de f sur g\n");
	PutString("f fini\n");
}

int main()
{
	int tidf, tidg;
	tidg = UserThreadCreate(g, 0);
	tidf = UserThreadCreate(f, &tidg);
	if(UserThreadJoin(tidg) == -1)
		PutString("Erreur join du main sur g\n");
	PutString("Fin du main\n");
    return 0;
}

#include "syscall.h"

/**
 * 	Le thread principal cree deux threads. Il attend ensuite ces deux threads avant de se terminer.
 * 	f etant plus long que g, au moment ou le thread principal attendra g, il sera deja fini.
 * 	Il n'y a pas d'erreur, le thread main se termine sans attendre g.
 *
 */

void g(void* arg)
{
	if(PutString("thread g\n") == -1)
		PutString("Erreur putstring\n");
	PutString("g fini\n");
}

void f(void* arg)
{
	int i;
	if(PutString("thread f\n") == -1)
		PutString("Erreur putstring\n");
	for (i = 0 ; i < 500000 ; i++)
	{
	}
	PutString("f fini\n");
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testJoinThreadDejaFini : \n");
	PutString("Cree 2 thread, attend le 1er puis le 2e, qui est deja fini a ce moment la.\n");
	PutString("-----------------------------------------\n");
	int tidf, tidg;
	int th_ret;
	tidg = UserThreadCreate(g, 0);
	tidf = UserThreadCreate(f, 0);
	if(UserThreadJoin(tidf, &th_ret) == -1)
		PutString("Erreur join sur f\n");
	if(UserThreadJoin(tidg, &th_ret) == -1)
		PutString("Erreur join sur g\n");
	PutString("Fin du main\n");
    return 0;
}

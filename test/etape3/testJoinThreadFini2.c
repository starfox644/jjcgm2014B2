#include "syscall.h"

/**
 * 	Le thread main cree deux threads qui executent f et g.
 *	Le main attend f, puis g, qui est deja termine.
 *
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
	for ( i = 0 ; i < 50 ; i++);
	if(PutString("thread f\n") == -1)
		PutString("Erreur putstring\n");
	PutString("f fini\n");
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testJoinThreadFini2 : \n");
	PutString("Lance 2 threads et les attend, le 2e attendu est deja termine.\n");
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

#include "syscall.h"

/**
 *	Lance 3 threads en parallele qui n'appellent pas UserThreadExit pour se terminer.
 */


void fin(void* arg)
{
	PutString("fin, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
}

void g(void* arg)
{
	PutString("g, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
}

void f(void* arg)
{
	PutString("f, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testFin : \n");
	PutString("Teste la terminaison des threads sans appel explicite a UserThreadExit.\n");
	PutString("-----------------------------------------\n");
	PutString("Main, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadCreate(f, 0);
	UserThreadCreate(g, 0);
	UserThreadCreate(fin, 0);
    return 0;
}

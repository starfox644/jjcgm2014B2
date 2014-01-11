#include "syscall.h"

/**
 *	Lance 3 threads en parallele et attend leur terminaison.
 *	Ce n'est qu'une fois tous les autres threads termines qu'il se termine a son tour.
 */


void fin(void* arg)
{
	PutString("Dernier thread\n");
	PutString("Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadExit();
}

void g(void* arg)
{
	PutString("Thread 2\n");
	PutString("Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadExit();
}

void f(void* arg)
{
	PutString("Thread 1\n");
	PutString("Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadExit();
}

int main()
{
	PutString("Thread main\n");
	PutString("Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadCreate(f, 0);
	UserThreadCreate(g, 0);
	UserThreadCreate(fin, 0);
	PutString("Le thread main a fini ! \n");
    return 0;
}

#include "syscall.h"

/**
 *	Lance 3 threads en parallele et attend leur terminaison.
 *	Le thread principal attend en faisant une attente active.
 */


void fin(void* arg)
{
	int* attFin = (int*)arg;
	PutString("Dernier thread\n");
	*attFin = 1;
	UserThreadExit();
}

void g(void* arg)
{
	int* attg = (int*)arg;
	PutString("Thread 2\n");
	*attg = 1;
	UserThreadExit();
}

void f(void* arg)
{
	int* attMain = (int*)arg;
	PutString("Thread 1\n");
	*attMain = 1;
	UserThreadExit();
}

int main()
{
	int attf = 0;
	int attFin = 0;
	int attg = 0;
	PutString("Thread main\n");
	UserThreadCreate(f, &attf);
	UserThreadCreate(g, &attg);
	UserThreadCreate(fin, &attFin);
	while(attf != 1);
	while(attg != 1);
	while(attFin != 1);
	PutString("Tous les threads ont fini ! \n");
    return 0;
}

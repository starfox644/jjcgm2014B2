#include "syscall.h"

/**
 *	Lance 3 threads en parallele et attend leur terminaison.
 *	Ce n'est qu'une fois tous les autres threads termines qu'il se termine a son tour.
 *	Le thread principal attend les autres via semaphore pris par ses threads encore en execution
 */


void fin(void* arg)
{
	PutString("Dernier thread\n");
	UserThreadExit(0);
}

void g(void* arg)
{
	PutString("Thread 2\n");
	UserThreadExit(0);
}

void f(void* arg)
{
	PutString("Thread 1\n");
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadParallele2 : \n");
	PutString("Lance 3 thread que le main va attendre puis se terminer.\n");
	PutString("-----------------------------------------\n");
	PutString("Thread main\n");
	UserThreadCreate(f, 0);
	UserThreadCreate(g, 0);
	UserThreadCreate(fin, 0);
	PutString("Le thread main a fini\n");
    return 0;
}

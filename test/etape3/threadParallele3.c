#include "syscall.h"

/**
 *	Le thread main lance le premier thread, qui lui meme lance un second thread...
 *	Le thread main attend la terminaison des autres avant de se terminer.
 */


void fin(void* arg)
{
	PutString("Thread 3 fini\n");
	UserThreadExit(0);
}

void g(void* arg)
{
	UserThreadCreate(fin, 0);
	PutString("Thread 2 fini\n");
	UserThreadExit(0);
}

void f(void* arg)
{
	UserThreadCreate(g, 0);
	PutString("Thread 1 fini\n");
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadParallele3 : \n");
	PutString("Lance un thread qui lance un autre thread etc. (cree 3 threads).\n");
	PutString("Tous les threads doivent se terminer avant que le processus s'arrete.\n");
	PutString("-----------------------------------------\n");
	UserThreadCreate(f, 0);
	PutString("Thread main fini\n");
    return 0;
}

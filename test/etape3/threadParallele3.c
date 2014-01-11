#include "syscall.h"

/**
 *	Le thread main lance le premier thread, qui lui meme lance un second thread...
 *	Le thread main attend la terminaison des autres avant de se terminer.
 */


void fin(void* arg)
{
	PutString("Dernier thread\n");
	UserThreadExit();
}

void g(void* arg)
{
	PutString("Thread 2\n");
	UserThreadCreate(fin, 0);
	UserThreadExit();
}

void f(void* arg)
{
	PutString("Thread 1\n");
	UserThreadCreate(g, 0);
	UserThreadExit();
}

int main()
{
	PutString("Thread main\n");
	UserThreadCreate(f, 0);
	PutString("Le thread main a fini ! \n");
    return 0;
}

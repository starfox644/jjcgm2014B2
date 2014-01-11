#include "syscall.h"

/**
 * 	Cree un thread qui cree un autre thread.
 */

void g(void* arg)
{
	int* attg = (int*)arg;
	PutString("Thread 2\n");
	*attg = 1;
	UserThreadExit();
}

void f(void* arg)
{
	int attg = 0;
	int* attMain = (int*)arg;
	PutString("Thread 1\n");
	UserThreadCreate(g, &attg);
	while(attg != 1);
	*attMain = 1;
	UserThreadExit();
}

int main()
{
	int attf = 0;
	PutString("Thread main\n");
	UserThreadCreate(f, &attf);
	while(attf != 1);
	PutString("Terminaison\n");
    return 0;
}

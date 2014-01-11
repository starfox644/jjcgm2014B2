#include "syscall.h"

/**
 * 	Premier test de thread. Cree un thread qui effectue un affichage.
 * 	Le thread principal attend ce thread et se termine.
 * 	Ce test est cense afficher "thread cree \n argument : 0"
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

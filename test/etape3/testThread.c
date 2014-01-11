#include "syscall.h"

/**
 * 	Premier test de thread. Cree un thread qui effectue un affichage.
 * 	Le thread principal attend ce thread et se termine.
 * 	Ce test est cense afficher "thread cree \n argument : 0"
 */

void f(void* arg)
{
	if(PutString("thread cree\n") == -1)
		PutString("Erreur putstring\n");
	else
	{
		if(PutString("argument : ") == -1)
			PutString("Erreur putstring\n");
		else
		{
			PutInt(*((int*)arg));
			PutChar('\n');
		}
	}
	*((int*)arg) = 1;
	UserThreadExit();
}

int main()
{
	int a = 0;
	UserThreadCreate(f, &a);
	while(a != 1);
    return 0;
}

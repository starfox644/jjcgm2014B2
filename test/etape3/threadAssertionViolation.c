#include "syscall.h"

/**
 *	Test pour la resolution de l'exception : Assertion Violation
 *	Le thread main cree un nouveau thread, puis chacun affiche une chaine, un caractere
 *	et un entier. Permet de voir la synchronisation et le changement de contexte
 */

void f(void* arg)
{
	int* a = (int*) arg;
	PutString("Thread 2\n");
	PutChar('b');
	PutString("\n");
	PutInt(2);
	PutString("\n");
	*a = 1;
	UserThreadExit();
}


int main()
{
	int a = 0;
	UserThreadCreate(f, &a);
	PutString("Thread main\n");
	PutChar('a');
	PutString("\n");
	PutInt(1);
	PutString("\n");
	while(a != 1);
    return 0;
}

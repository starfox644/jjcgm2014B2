#include "syscall.h"

/**
 *	Test pour la resolution de l'exception : Assertion Violation
 *	Le thread main cree un nouveau thread, puis chacun affiche une chaine, un caractere
 *	et un entier. Permet de tester la synchronisation des entrees sorties.
 */

void f(void* arg)
{
	PutString("Thread 2\n");
	PutChar('b');
	PutString("\n");
	PutInt(2);
	PutString("\n");
	UserThreadExit(0);
}


int main()
{
	UserThreadCreate(f, 0);
	PutString("Thread main\n");
	PutChar('a');
	PutString("\n");
	PutInt(1);
	PutString("\n");
    return 0;
}

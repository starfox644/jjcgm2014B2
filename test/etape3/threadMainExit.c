#include "syscall.h"

/**
 * 	Permet de tester le comportement du programme lorsque le thread principal
 * 	appelle UserThreadExit avec une valeur de retour.
 */

int main()
{
	UserThreadExit(5);
    return 0;
}

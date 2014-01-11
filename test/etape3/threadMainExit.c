#include "syscall.h"

/**
 * 	Permet de tester le comportement du programme lorsque le thread principal
 * 	appelle UserThreadExit.
 */

int main()
{
	UserThreadExit();
    return 0;
}

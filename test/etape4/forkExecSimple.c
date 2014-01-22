/**
 * Test simple du ForkExec : cree un processus qui execute
 * progSimple (affichage de "Hello World !") et se termine
 */

#include "syscall.h"

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test forkExecSimple : \n");
	PutString("affiche \"Hello World !\" dans un autre processus\n");
	PutString("-----------------------------------------\n");
	ForkExec("./etape4/progSimple");
	return 0;
}

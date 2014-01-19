/**
 * Test simple du ForkExec : cree un processus qui execute
 * progSimple (affichage de "Hello World !") et se termine
 */

#include "syscall.h"

int main()
{
	ForkExec("./etape4/progSimple");
	PutString("\nFin de fork exec !\n");
	return 0;
}

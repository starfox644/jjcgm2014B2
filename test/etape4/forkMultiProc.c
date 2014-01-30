#include "syscall.h"

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("A lancer avec -rs\n");
	PutString("Lancement du test forkMultiProc : \n");
	PutString("Lance 5 processus executant un programme different.\n");
	PutString("-----------------------------------------\n");
	if (ForkExec("./etape2/putstring")==-1)
		PutString("Erreur creation processus\n");
	if (ForkExec("./etape2/putInt")==-1)
		PutString("Erreur creation processus\n");
	if (ForkExec("./etape2/putchar")==-1)
		PutString("Erreur creation processus\n");
	if (ForkExec("./etape2/putstringNumber")==-1)
		PutString("Erreur creation processus\n");
	if (ForkExec("./etape4/forkExecSimple")==-1)
		PutString("Erreur creation processus\n");
	return 0;
}

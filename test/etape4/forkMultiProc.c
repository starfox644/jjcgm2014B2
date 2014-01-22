#include "syscall.h"

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test forkMultiProc : \n");
	PutString("Lance 5 processus executant un programme different.\n");
	PutString("-----------------------------------------\n");
	PutString("\n***************************\n");
	ForkExec("./etape2/putstring");
	PutString("\n***************************\n");
	ForkExec("./etape2/putInt");
	PutString("\n***************************\n");
	ForkExec("./etape2/putchar");
	PutString("\n***************************\n");
	ForkExec("./etape2/putstringNumber");
	PutString("\n***************************\n");
	ForkExec("./etape4/forkExecSimple");
	PutString("\n***************************\n");
	return 0;
}

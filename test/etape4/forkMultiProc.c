#include "syscall.h"

int main()
{
	PutString("\n---------------------------\n");
	ForkExec("./etape2/putstring");
	PutString("\n---------------------------\n");
	ForkExec("./etape2/putInt");
	PutString("\n---------------------------\n");
	ForkExec("./etape2/putchar");
	PutString("\n---------------------------\n");
	ForkExec("./etape2/putstringNumber");
	PutString("\n---------------------------\n");
	ForkExec("./etape4/forkExecSimple");
	PutString("\n---------------------------\n");
	return 0;
}

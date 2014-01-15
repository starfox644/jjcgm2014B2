#include "syscall.h"

int main()
{
//	PutString("Pid : ");
//	PutInt(GetPid());
	ForkExec("./etape4/progSimple");
	PutString("\nFin du process principal\n");
	return 0;
}

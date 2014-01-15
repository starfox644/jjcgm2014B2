#include "syscall.h"

int main()
{
	ForkExec("./etape4/progSimple");
	PutString("Fin du process principal\n");
	return 0;
}

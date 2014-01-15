#include "syscall.h"

int main()
{
	int tid;
	tid = ForkExec("./etape4/progSimple");
	PutString("Fin du process principal\n");
	return 0;
}

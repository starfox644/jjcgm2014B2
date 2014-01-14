#include "syscall.h"

int main()
{

	int error;
	PutString("Avant le ForkExec\n");
	error = ForkExec("./etape4/progSimple");
	PutInt(error);
	PutString("Apres le ForkExec\n");
	return 0;
}

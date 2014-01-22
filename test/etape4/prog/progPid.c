#include "syscall.h"

// ecrit le pid du processus courant
int main()
{
	PutString("PID : ");
	PutInt(GetPid());
	PutString("\n");
    return 0;
}

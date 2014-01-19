#include "syscall.h"

// ecrit le nombre 546 et affiche le nombre de caracteres ecrits
int main()
{
	PutString("PID : ");
	PutInt(GetPid());
    return 0;
}

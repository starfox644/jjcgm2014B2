#include "syscall.h"

int main()
{
	int nb = PutInt(546);
	PutChar('\n');
	PutString("Nombre de caracteres ecrits : ");
	PutInt(nb);
	PutChar('\n');
    return 0;
}

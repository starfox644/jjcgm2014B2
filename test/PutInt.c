#include "syscall.h"

int main()
{
	int nb = PutInt(546);
	PutString("Nombre de caracteres ecrits : ");
	PutInt(nb);
	PutChar('\n');
    return 0;
}

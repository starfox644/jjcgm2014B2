#include "syscall.h"

// ecrit le nombre 546 et affiche le nombre de caracteres ecrits
int main()
{
	int nb = PutInt(546);
	PutChar('\n');
	PutString("Nombre de caracteres ecrits : ");
	PutInt(nb);
	PutChar('\n');
    return 0;
}

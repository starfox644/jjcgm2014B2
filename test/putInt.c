#include "syscall.h"

int main()
{
	// ecrit le nombre 546 et affiche le nombre de caracteres ecris
	int nb = PutInt(546);
	PutChar('\n');
	PutString("Nombre de caracteres ecrits : ");
	PutInt(nb);
	PutChar('\n');
    return 0;
}

/**
 * Ce programme demande un caractere a l'utilisateur, cree un thread
 * en le lui donnant en parametre. Le thread cree affiche le caractere
 * a l'ecran et termine.
 */

#include "syscall.h"

typedef struct Arg {
	volatile int  wait;
	char a;
} Arg;

void f(void* arg)
{
	Arg *param = (Arg*) arg;
	PutChar(param->a);
	param->wait = 0;
	UserThreadExit();
}

int main()
{
	Arg arg;
	PutString("Entrez un caractere :");
	arg.wait = 1;
	arg.a = GetChar();
	UserThreadCreate(f, &arg);
	while(arg.wait);
	PutChar('\n');
    return 0;
}

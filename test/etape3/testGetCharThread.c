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
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testGetCharThread : \n");
	PutString("Demande un char et l'affiche via un autre thread.\n");
	PutString("-----------------------------------------\n");
	Arg arg;
	PutString("Entrez un caractere :");
	arg.wait = 1;
	arg.a = GetChar();
	UserThreadCreate(f, &arg);
	while(arg.wait);
	PutChar('\n');
    return 0;
}

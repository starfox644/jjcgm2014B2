/**
 * Ce programme demande un caractere a l'utilisateur, cree un thread
 * en le lui donnant en parametre. Le thread cree affiche le caractere
 * a l'ecran et termine. Des appels a PutChar concurentiels sont effectues
 * pour tester la synchronisation
 */


#include "syscall.h"

typedef struct Arg {
	int  wait;
	char a;
} Arg;

void f(void* arg)
{
	PutChar('5');
	Arg *param = (Arg*) arg;
	PutChar(param->a);
	PutChar('6');
	param->wait = 0;
	PutChar('7');
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testGetCharMultiThread : \n");
	PutString("Affiche plusieurs caracteres sur 2 threads differents pour tester la synchro\n");
	PutString("-----------------------------------------\n");
	Arg arg;
	PutChar('1');
	arg.wait = 1;
	PutChar('2');
	PutString("Entrez un caractere :");
	arg.a = GetChar();
	PutChar('1');
	UserThreadCreate(f, &arg);
	PutChar('2');
	while(arg.wait)
		PutChar(' ');
	PutChar('8');
    return 0;
}

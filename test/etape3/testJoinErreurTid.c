#include "syscall.h"
#include "syscall.h"

/**
 * Le thread principal cree un thread qui effectue un affichage, puis fait un join
 * sur un tid inexistant.
 */

void f(void* arg)
{
	if(PutString("thread f\n") == -1)
		PutString("Erreur putstring\n");
}

int main()
{
	int tidf;
	tidf = UserThreadCreate(f, 0);
	if(UserThreadJoin(6) == -1)
		PutString("Erreur join\n");
	PutString("Fin du main\n");
    return 0;
}

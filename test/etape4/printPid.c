#include "syscall.h"

// ecrit le pid du processus courant
int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test printPid : affiche le PID.\n");
	PutString("-----------------------------------------\n");
	PutString("PID : ");
	PutInt(GetPid());
	PutString("\n");
    return 0;
}

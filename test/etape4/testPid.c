#include "syscall.h"
#include "../nachos_libc/Printf.h"
#define NB_PROC 5

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testPid : \n");
	PutString("Cree 5 processus et affiche leur PID.\n");
	PutString("-----------------------------------------\n");
	int i;
	int pid;
	for(i = 0 ; i  < NB_PROC ; i++)
	{
		pid = ForkExec("etape4/prog/progPid");
		if(pid == -1)
		{
			PutString("Erreur de creation de processus\n");
		}
	}
    return 0;
}

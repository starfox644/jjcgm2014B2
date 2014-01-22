#include "syscall.h"
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
			PutString("erreur de creation de processus\n");
		}
		else
		{
			PutString("pid du processus :\n");
			PutInt(pid);
		}
	}
    return 0;
}

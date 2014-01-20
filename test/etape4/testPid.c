#include "syscall.h"
#define NB_PROC 5

int main()
{
	int i;
	int pid;
	for(i = 0 ; i  < NB_PROC ; i++)
	{
		pid = ForkExec("etape4/printPid");
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

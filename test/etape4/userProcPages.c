#include "syscall.h"


int main()
{
	int pid = 0, pid1 = 0;
	PutString("\n");
	PutString("Lancement de userpages0\n");
	pid = ForkExec("etape4/userpages0");
	if(pid == -1){
		PutString("Erreur du lancement de userpages0\n");
		return -1;
	}
	PutString("\n");
	pid1 = ForkExec("etape4/userpages1");
	if(pid1 == -1){
		PutString("Erreur du lancement de userpages1\n");
		return -1;
	}
	PutString("\n");
    return 0;
}


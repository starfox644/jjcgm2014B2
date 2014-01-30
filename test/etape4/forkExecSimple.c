/**
 * Test simple du ForkExec : cree un processus qui execute
 * progSimple (affichage de "Hello World !") et se termine
 */

#include "syscall.h"

int main()
{
	int pid, error;
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test forkExecSimple : \n");
	PutString("affiche \"Hello World !\" dans un autre processus\n");
	PutString("-----------------------------------------\n");
	pid = ForkExec("./etape4/prog/progSimple");
	if(pid == -1){
		PutString("Erreur de création du processus");
		PutString("\n");
		return -1;
	}

	error = WaitPid(pid);
	if(error == -1){
		PutString("Erreur d'attente du processus");
		PutString("\n");
		return -1;
	}

	return 0;
}

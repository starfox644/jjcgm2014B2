/*
 * waitPidAll.c
 *	Ce programme teste tous les cas d'executions/d'erreurs de WaitPid
 *  Created on: 20 janv. 2014
 *      Author: Sarkal
 */

#include "syscall.h"

int main(){
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test waitPidAll : \n");
	PutString("Teste les cas d'executions/erreurs de waitpid.\n");
	PutString("-----------------------------------------\n");
	int error;
	int pid;

	// On lance le processus a attendre
	pid = ForkExec("./etape4/prog/progWait");
	if(pid == -1){
		PutString("[Test1] Erreur de création du processus\n");
	}

	// On lance le processus qui va attendre le precedent
	pid = ForkExec("./etape4/prog/progDoubleAttente");
	if(pid == -1){
		PutString("[Test1] Erreur de création du processus\n");
		return -1;
	}

	// On attend le processus, en concurrence avec le precedent
	PutString("********************************************\n");
	PutString("[Test1] WaitPid conccurentiel : Mise en attente 1\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[Test1] Erreur - Ce processus est deja attendu par un autre processus\n");
		return -1;
	}
	else
	{
		PutString("[Test1] Succes\n");
	}

	// On attend le processus deja termine
	PutString("********************************************\n");
	PutString("[Test1] WaitPid sur un processus deja termine\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[Test1] Erreur - Ce processus est deja attendu par un autre processus\n");
		PutString("********************************************\n");
		return -1;
	}
	PutString("[Test1] Succes\n");
	PutString("********************************************\n");

	// On attend le processus, en concurrence avec le precedent
	PutString("[Test1] WaitPid sur un processus termine par une erreur\n");
	error = WaitPid(2);
	if(error == -1){
		PutString("[Test1] Erreur - Ce processus est deja attendu par un autre processus\n");
		PutString("********************************************\n");
		return -1;
	}
	PutString("[Test1] Succes\n");
	PutString("********************************************\n");
	// On dit au process de s'attendre lui meme
	PutString("[Test1] WaitPid sur soi meme\n");
	error = WaitPid(GetPid());
	if(error == -1){
		PutString("[Test1] Erreur - Impossible de s'attendre soi-meme !\n");
		PutString("********************************************\n");
	}
	else
	{
		PutString("[Test1] Echec\n");
		PutString("********************************************\n");
	}
	PutString("[Test1] Les processus fils sont arretes donc je peux m'arreter aussi.\n");
	return 0;
}


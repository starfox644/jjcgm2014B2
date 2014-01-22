/*
 * progDoubleAttente.c
 *	Ce programme est appele par waitPidDoubleAttente pour attendre en concurrence le processus 1
 *  Created on: 20 janv. 2014
 *      Author: Sarkal
 */

#include "syscall.h"

int main(){
	int error;
	PutString("[Test2] WaitPid conccurentiel : Mise en attente 2\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[Test2] Erreur - Ce processus est deja attendu par un autre processus\n");
		return -1;
	}
	else
	{
		PutString("[Test2] Succes\n");
	}

	// On attend le processus deja termine
	PutString("********************************************\n");
	PutString("[Test2] WaitPid sur un processus deja termine\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[Test2] Echec\n");
		PutString("********************************************\n");
		return -1;
	}
	PutString("[Test2] Succes\n");
	PutString("********************************************\n");

	// On attend le processus, en concurrence avec le precedent
	PutString("[Test2] WaitPid sur un processus termine par une erreur\n");
	error = WaitPid(0);
	if(error == -1){
		PutString("[Test2] Echec\n");
		PutString("********************************************\n");
		return -1;
	}
	PutString("[Test2] Succes\n");
	PutString("********************************************\n");
	// On dit au process de s'attendre lui meme
	PutString("[Test2] WaitPid sur soi meme\n");
	error = WaitPid(GetPid());
	if(error == -1){
		PutString("[Test2] Erreur - Impossible de s'attendre soi-meme !\n");
		PutString("********************************************\n");
	}
	else
	{
		PutString("[Test2] Echec\n");
		PutString("********************************************\n");
	}
	return 0;
}


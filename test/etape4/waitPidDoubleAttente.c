/*
 * waitPidMulti.c
 *	Ce programme lance plusieurs processus et les attends avec waitpid()
 *	/!\ Pas encore possible parce qu'on a pas de communication entre processus. /!\
 *  Created on: 20 janv. 2014
 *      Author: Sarkal
 */

#include "syscall.h"

int main(){
	int error;
	int pid;
	PutString("Lancement du programme de test waitPidMulti\n");

	pid = ForkExec("./etape4/progWait");
	if(pid == -1){
		PutString("[WaitPidDoubleAttente] Erreur de création du processus\n");
	}

	pid = ForkExec("./etape4/progDoubleAttente");
	if(pid == -1){
		PutString("[WaitPidDoubleAttente] Erreur de création du processus\n");
		return -1;
	}

	error = WaitPid(1);
	if(error == -1){
		PutString("[WaitPidDoubleAttente] Erreur d'attente du processus\n");
		return -1;
	}
	PutString("Les processus fils sont arretes donc je peux m'arreter aussi.\n");
	PutString("Au revoir!\n");
	return 0;
}


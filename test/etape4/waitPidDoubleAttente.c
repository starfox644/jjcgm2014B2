/*
 * waitPidDoubleAttente.c
 *	Ce programme lance un processus qu'il va attendre, un processus qui va aussi attendre ce dernier
 *	puis attend le 1er processus.
 *  Created on: 20 janv. 2014
 *      Author: Sarkal
 */

#include "syscall.h"

int main(){
	int error;
	int pid;
	PutString("Lancement du programme de test waitPidMulti\n");

	// On lance le processus a attendre
	pid = ForkExec("./etape4/progWait");
	if(pid == -1){
		PutString("[WaitPidDoubleAttente] Erreur de création du processus\n");
	}

	// On lance le processus qui va attendre le precedent
	pid = ForkExec("./etape4/progDoubleAttente");
	if(pid == -1){
		PutString("[WaitPidDoubleAttente] Erreur de création du processus\n");
		return -1;
	}

	// On attend le processus, en concurrence avec le precedent
	PutString("[WaitPidDoubleAttente] Mise en attente\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[WaitPidDoubleAttente] Erreur - Ce processus est deja attendu par un autre processus\n");
		return -1;
	}
	PutString("[WaitPidDoubleAttente] Fin attente\n");
	PutString("Les processus fils sont arretes donc je peux m'arreter aussi.\n");
	PutString("Au revoir!\n");
	return 0;
}


/*
 * progDoubleAttente.c
 *	Ce programme est appele par waitPidDoubleAttente pour attendre en concurrence le processus 1
 *  Created on: 20 janv. 2014
 *      Author: Sarkal
 */

#include "syscall.h"

int main(){
	int error;
	PutString("[progDoubleAttente] Mise en attente\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[progDoubleAttente] Erreur - Ce processus est deja attendu par un autre processus\n");
		return -1;
	}
	PutString("[progDoubleAttente] Fin attente\n");
	return 0;
}


/*
 * progDoubleAttente.c
 *	Ce programme est appele par waitPidDoubleAttente pour attendre le processus 1
 *  Created on: 20 janv. 2014
 *      Author: Sarkal
 */

#include "syscall.h"

int main(){
	int error;
//	PutString("2 - Mise en attente\n");
	error = WaitPid(1);
	if(error == -1){
		PutString("[progDoubleAttente] Erreur d'attente du processus\n");
		return -1;
	}
//	PutString("2 - Fin attente\n");
	return 0;
}


/*
 * userProcMulti.c
 *
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_PROC 12

int main(){

	int pid[MAX_PROC], i;
	PutString("Lancement du programme de test userProcMulti\n");
	for(i = 0; i < MAX_PROC; i++){
		PutString("Lancement du proc numéro : ");
		PutInt(i);
		PutString("\n");
		pid[i] = ForkExec("../etape4/userThreadMulti");
	}

	return 0;

}


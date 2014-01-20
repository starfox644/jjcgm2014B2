/*
 * waitPidMulti.c
 *	Ce programme lance plusieurs processus et les attends avec waitpid()
 *  Created on: 17 janv. 2014
 *      Author: Sarkal
 */
#include "syscall.h"
#define NB_PROCESS 4

int main(){
	int pid[NB_PROCESS], error, i;
	PutString("Lancement du programme de test waitPidMulti\n");

	for(i = 0; i < NB_PROCESS; i++)
	{
		pid[i] = ForkExec("./etape4/progWait");
		if(pid[i] == -1){
			PutString("Erreur de création du processus #");
			PutInt(i);
			PutChar('\n');
			return -1;
		}
	}
	for(i = 0; i < NB_PROCESS; i++)
	{
		error = WaitPid(pid[i]);
		if(error == -1){
			PutString("Erreur d'attente du processus #");
			PutInt(i);
			PutChar('\n');
			return -1;
		}
	}
	PutString("Les processus fils sont arretes donc je peux m'arreter aussi.\n");
	PutString("Au revoir!\n");
	return 0;

}


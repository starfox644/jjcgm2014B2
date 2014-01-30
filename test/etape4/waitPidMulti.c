/*
 * waitPidMulti.c
 *	Ce programme lance plusieurs processus et les attends avec waitpid()
 *  Created on: 17 janv. 2014
 *      Author: Sarkal
 */
#include "syscall.h"
#define NB_PROCESS 4

int main(){
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test waitPidMulti : \n");
	PutString("Lance plusieurs processus et attend leur terminaison.\n");
	PutString("-----------------------------------------\n");
	int pid[NB_PROCESS], error, i;

	for(i = 0; i < NB_PROCESS; i++)
	{
		pid[i] = ForkExec("./etape4/prog/progWait");
		if(pid[i] == -1){
			PutString("Erreur de création du processus #");
			PutInt(i+1);
			PutChar('\n');
		}
	}
	for(i = 0; i < NB_PROCESS; i++)
	{
		error = WaitPid(pid[i]);
		if(error == -1){
			PutString("Erreur d'attente du processus #");
			PutInt(i);
			PutChar('\n');
		}
	}
	PutString("Les processus fils sont arretes donc je peux m'arreter aussi.\n");
	PutString("Au revoir!\n");
	return 0;

}


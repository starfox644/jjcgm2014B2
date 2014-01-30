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
	PutString("La creation d'un processus peut echouer si memoire insuffisante.\n");
	PutString("-----------------------------------------\n");
	int pid[NB_PROCESS], error, i, retour = 0;

	for(i = 0; i < NB_PROCESS; i++)
	{
		pid[i] = ForkExec("./etape4/prog/progWait");
		if(pid[i] == -1){
			PutString("Erreur de création du processus #");
			PutInt(i+1);
			PutChar('\n');
			retour = -1;
		}
	}
	for(i = 0; i < NB_PROCESS; i++)
	{
		if (pid[i] != -1)
		{
			error = WaitPid(pid[i]);
			if(error == -1){
				PutString("Erreur d'attente du processus #");
				PutInt(i);
				PutChar('\n');
				retour = -1;
			}
		}
	}
	PutString("Les processus fils sont arretes, arret.\n");
	return retour;

}


/*
 * userProcMulti.c
 *	Ce programme permet de lancer un processus pour ensuite attendre sa réalisation avant de s'arreter.
 *	Nous appelrons le programme userpages0 pour ce test
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"

int main(){


	int pid, error;
	PutString("Lancement du programme de test userProcWait\n");
	PutString("\n");
	pid = ForkExec("./etape4/forkMultiProc");
	PutString("Pid : ");
	PutInt(pid);
	PutString("\n");
	if(pid == -1){
		PutString("Erreur de création du processus");
		PutString("\n");
		return -1;
	}
	error = WaitPid(pid);
	if(error == -1){
		PutString("Erreur d'attente du processus");
		PutString("\n");
		return -1;
	}

	PutString("Le processus fils est arrété donc je peux m'arréter aussi.\n");
	PutString("Au revoir!");
	return 0;

}


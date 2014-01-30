/*
 * userProcMulti.c
 *	Ce programme permet de lancer un processus pour ensuite attendre sa réalisation avant de s'arreter.
 *	Nous appelrons le programme userpages0 pour ce test
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"

int main(){

	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test userProcWait : \n");
	PutString("Cree un processus qui cree plusieurs processus et attend sa terminaison.\n");
	PutString("-----------------------------------------\n");

	int pid, error;
	PutString("Lancement du programme de test userProcWait\n");
	PutString("\n");
	pid = ForkExec("./etape4/forkMultiProc");
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

	PutString("Le processus fils est arrete.\n");
	return 0;

}


/*
 * Eval.c
 *	Ici nous allons étudier la ligne de commande entrée dans notre shell et faire ce qui correspond.
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */
#include "shell.h"
int commandIntegre(char *cmdLine);

void eval(char* cmdLine){
	int pid // contient le numéro de processus créé.
	int newProc, error;
	if(cmdLine == 0){// on ne tient pas compte des lignes vide
		return ;
	}
	//on teste que ce ne soit pas une commande intégré.
	if(!commandIntegre(cmdLine)){
		//pour le moment on part du principe qu'on ne fait que des taches en premier plan et rien en tache de fond.
		newProc = ForkExec(cmdLine); //on crée notre executable
		if(newProc == -1){
			PutString("Erreur de création de notre processus\n");
			return;
		}
		//on attend que l'executable ce finisse pour retourne au shell.
		error = WaitPid (newProc);
		if(error == -1){
			PutString("Erreur lors de l'attente du processus fils");
			return;
		}
	}
	return;
}

int commandIntegre(char * cmdLine){
	if(StrCmp(cmdLine,"quit"){
		Exit(0);	//commande pour arreter le shell
	}
	//pour le moment pas de ligne intégré autre que la commande quitter
	return 0;
}

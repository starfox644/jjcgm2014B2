/*
 * Eval.c
 *	Ici nous allons étudier la ligne de commande entrée dans notre shell et faire ce qui correspond.
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */
#include "my_shell.h"
/*int commandIntegre(char *cmdLine);

void eval(char* cmdLine){
	int pid; // contient le numéro de processus créé.
	int newProc, error;

	if(cmdLine[0] == 0){// on ne tient pas compte des lignes vide
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
	if(StrCmp(cmdLine,"quit"){ //commande pour arreter le shell
		Exit(0);
	}
	if(StrCmp(cmdLine,"jobs"){// on fait l'affichage des processus actif
		AfficherJobs();
	}
	if(StrCmp(cmdLine,"help"){//on affiche l'aide de notre shell
		PutString("Bienvenue dans l'aide de NachOS_Shell\n\n");
		PutString("-Pour afficher les programmes en cours : \n");
		PutString("\tTapez jobs\n");
		PutString("-Pour quitter NachOS_Shell : \n");
		PutString("\tTapez quit\n");
		PutString("Merci d'utiliser NachOS_Shell!\n");
		PutString("A bientot\n");

	}
	//pour le moment pas de ligne intégré autre que la commande quitter
	return 0;
}*/

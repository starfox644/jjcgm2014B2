/*
 * Eval.c
 *	Ici nous allons étudier la ligne de commande entrée dans notre shell et faire ce qui correspond.
 * Récupération du code de notre APNEE de SR de Licence 3 modifé avec les commande pour NachOS
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#include "eval.h"

void evalLine(char* cmdLine){
	int newProc, error;
	//tableau d'argument
	char* argv[MAXARGS];	//argv pour la création de nos programme
	int background;
	int pid;
	background = parseline(cmdLine,argv);
	if(argv[0] == 0){// on ne tient pas compte des lignes vide
			return ;
	}
	//on teste que ce ne soit pas une commande intégré.
	if(commandIntegre(argv) != 0){
		//pour le moment on part du principe qu'on ne fait que des taches en premier plan et rien en tache de fond.
		/*
		 * Partie a modifier pour gérer les arguments.
		 */
		newProc = ForkExec(cmdLine); //on crée notre executable
		pid = GetPid();
		add_job(pid,background,cmdLine);	//on l'ajoue a notre tableau
		if(newProc == -1){
			PutString("Programme non connu\n");
			return;
		}
		//on attend que l'executable ce finisse pour retourne au shell.
		error = WaitPid (newProc);
		supp_job_pid(newProc);	//on enleve le job de notre tableau
		if(error == -1){
			PutString("Erreur lors de l'attente du processus fils\n");
			return;
		}
	}
	return;
}

int commandIntegre(char **argv){
	if(StrCmp(argv[0],"quit") == 1){ //commande pour arreter le shell
		Exit(0);
	}
	if(StrCmp(argv[0],"jobs") == 1){// on fait l'affichage des processus actif
		afficherJobs();
		return 0;
	}
	if(StrCmp(argv[0],"ls") == 1){//on affiche ce qui ce trouve dans le répétoire courant
		PutString("Non implémenté :D\n");
		return 0;
	}
	if(StrCmp(argv[0],"pwd") == 1){//affiche le chemin du répertoire courant
		PutString("Non implémenté :D\n");
		return 0;
	}
	if(StrCmp(argv[0],"help") == 1){//on affiche l'aide de notre shell
		Printf("Bienvenue dans l'aide de NachOS_Shell\n\n",0);
		Printf("-Pour afficher les programmes en cours tapez :",0);
		Printf(" jobs\n",0);
		Printf("-Pour afficher le contenu du répertoire courant tapez :",0);
		Printf(" ls\n",0);
		Printf("-Pour afficher le chemin du répertoire courant tapez :",0);
		Printf(" pwd\n",0);
		Printf("-Pour quitter NachOS tapez :",0);
		Printf(" quit\n\n",0);
		Printf("Merci d'utiliser NachOS_Shell!\n",0);
		Printf("\t\tA bientôt.\n",0);
		return 0;

	}
	//pour le moment pas de ligne intégré autre que la commande quitter
	return 1;
}

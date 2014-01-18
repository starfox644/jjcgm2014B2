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
		if(newProc == -1){
			PutString("Programme non connu\n");
			return;
		}
		add_job(newProc,background,cmdLine);	//on l'ajoute a notre tableau

		if(!background){// travaille de premier plan on attend le fils
			//on attend que l'executable ce finisse pour retourne au shell.
			error = WaitPid (newProc);
			if(error == -1){
				PutString("Erreur lors de l'attente du processus fils\n");
				return;
			}
		}else{
			Printf("Travail d'arriere plan %s\n",cmdLine);	//travail d'arriere plan on affiche sa commande
		}
		supp_job_pid(newProc);	//on enleve le job de notre tableau

	}
	return;
}

int commandIntegre(char **argv){
	if(StrCmp(argv[0],"quit") == 1){ //commande pour arreter le shell
		Exit(0);
	}
	if(StrCmp(argv[0],"&") == 1){ //on ignore le & tout seul
		return 1;
	}
	if(StrCmp(argv[0],"jobs") == 1){// on fait l'affichage des processus actif
		afficherJobs();
		return 0;
	}
	if(StrCmp(argv[0],"ps") == 1){// on fait l'affichage des processus actif
			afficherProcess();
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
	/* utile pour lorsqu'il y aura le fileSys*/
	if(StrCmp(argv[0],"ls") == 1){//on affiche ce qui ce trouve dans le répétoire courant
			PutString("Non implémenté :D\n");
			return 0;
		}
		if(StrCmp(argv[0],"pwd") == 1){//affiche le chemin du répertoire courant
			PutString("Non implémenté :D\n");
			return 0;
		}
	//ce n'est pas une ligne intégré donc on retourne 1
	return 1;
}

void afficherProcess(){
	int *liste_Process;
	int nbProcessus = GetNbProcess(), i = 0 ;
	int compteur = 0;
	//on alloue la mémoire et on recupere la liste des processus
	liste_Process = (int*) malloc((nbProcessus * 100) * sizeof(int));
	if(liste_Process == 0){
		Printf("arf\n",0);
	}
	liste_Process = GetListProcess(liste_Process);
	//on réalise l'affichage
	Printf("Liste des processus sytème\n",0);
	while(compteur < nbProcessus){ //pour chaque processus faire
		Printf("Pid du processus : %d",(void*)liste_Process[i]);
		i++;
		Printf("\t\t Etat : %d\n",(void*)liste_Process[i]);
		i++;
		compteur++;
	}
	return;
}

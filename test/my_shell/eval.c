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
	int retour;

	if(StrCmp(argv[0],"quit") == 1){ //commande pour arreter le shell
		Printf("\n\tMerci d'avoir utilisé NachOS_Shell!\n");
		Printf("\tA bientôt.\n\n");
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
		Printf("- afficher les programmes en cours : jobs\n");
		Printf("- afficher les processus système en cours : ps\n");
		Printf("- afficher le contenu du répertoire courant : ls\n");
		Printf("- afficher le chemin du répertoire courant : pwd\n");
		Printf("- afficher le contenu d'un fichier : cat <fichier>\n");
		Printf("- copier un fichier Unix vers un fichier NachOS : cp <source> <destination>\n");
		Printf("- supprimer un fichier NachOS : rm <fichier>\n");
		Printf("- afficher le système de fichier dans son ensemble : dir\n");
		Printf("- naviguer dans le système de fichier : cd <dossier>\n");
		Printf("- avoir un fichier d'aide sur les commandes : man <commande>\n");
		Printf("- creer un dossier : mkdir <dossier>\n");
		Printf("- supprimer un dossier : rmdir <dossier>\n");
		Printf("- creer un fichier : create <fichier>\n");
		Printf("- savoir si un chemin existe : exist <chemin>\n");
		Printf("- quitter NachOS : quit\n\n");
		return 0;


	}
	/* utile pour lorsqu'il y aura le fileSys*/
	if(StrCmp(argv[0],"ls") == 1){//on affiche ce qui ce trouve dans le répétoire courant
		ls();
		return 0;
	}
	if(StrCmp(argv[0],"pwd") == 1){//affiche le chemin du répertoire courant
		char* buf = malloc(MAX_CHAR * sizeof(char));
		pwd(buf);
		PutString(buf);
		PutString("\n");
		free(buf);
		return 0;
	}
	if(StrCmp(argv[0],"cp") == 1){//copie un fichier unix vers un fichier nachOS
		copy(argv[1],argv[2]);
		return 0;
	}
	if(StrCmp(argv[0],"rm") == 1){//supprime un fichier nachOS
		retour = rm(argv[1]);
		if (retour)
			PutString("Echec de suppression\n");
		return 0;
	}
	if(StrCmp(argv[0],"dir") == 1){//affiche l'integralité du filesystem
		PutString("Non implémenté :D\n");
		//afficherSysteme();
		return 0;
	}
	if(StrCmp(argv[0],"cd") == 1){//permet de naviger dans le système de fichier
		cd(argv[1]);
		return 0;
	}
	if(StrCmp(argv[0],"man") == 1){//affiche des pages d'aide
		PutString("Non implémenté :D\n");
		//afficherManuel(argv[1]);
		return 0;
	}
	if(StrCmp(argv[0],"mkdir") == 1){//affiche des pages d'aide
		retour = mkdir(argv[1]);
		if (retour)
			PutString("Echec de creation\n");
		return 0;
	}
	if(StrCmp(argv[0],"rmdir") == 1){//affiche des pages d'aide
		retour = rmdir(argv[1]);
		if (retour)
			PutString("Echec de suppression\n");
		return 0;
	}
	if(StrCmp(argv[0],"exist") == 1){//affiche des pages d'aide
		retour = pathExists(argv[1]);
		if (retour == -1)
			PutString("Erreur\n");
		else if (retour)
			PutString("Existe\n");
		else
			PutString("N'existe pas\n");
		return 0;
	}
	if(StrCmp(argv[0],"create") == 1){//affiche des pages d'aide
		retour = Create(argv[1], Atoi(argv[2]));
		if (retour)
			PutString("Echec de creation\n");
		return 0;
	}
	if(StrCmp(argv[0],"cat") == 1){//affiche des pages d'aide
			retour = Cat(argv[1]);
			if (retour)
				PutString("Erreur\n");
			return 0;
		}
	if(StrCmp(argv[0],"ifconfig") == 1){//permet d'afficher les informations concernant le réseau
		ifconfig();
		return 0;
	}
	//ce n'est pas une ligne intégré donc on retourne 1
	return 1;
}

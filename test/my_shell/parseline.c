/*
 * parseline.c
 *	Récupération du code de notre APNEE de SR de Licence 3 modifé avec les commande pour NachOS
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#include "parseline.h"

int parseline (char* cmdLine, char **argv){
	char *delim;	//pointe vers le premier delimiteur espace
	int argc;	//nombre d'arguments
	int bg; // savoir si un programme s'execute en arrière plan ou non (a voir si on s'en sers pour nachos)

	cmdLine[StrLengh(cmdLine)] = ' ';	//remplacer le '\n' final par espace

	while(*cmdLine && (*cmdLine == ' ')){	//ignorer les espaces au début
		cmdLine++;
	}
	argc = 0;

	while((delim = StrChr(cmdLine,' '))){// construis la liste des arguments
		argv[argc++] = cmdLine;
		*delim = '\0';
		cmdLine = delim + 1;
		while(*cmdLine && (*cmdLine == ' ')){	//ignore les espaces
			cmdLine++;
		}
	}
	argv[argc] = 0;	//termine la liste d'arguments
	if(argc == 0){	//ignorer ligne vide
		return 1;
	}
	if((bg = (*argv[argc-1] == '&')) != 0){//travail d'arriere-plan ?
			argv[--argc] = 0;
	}
	return bg;	//1 si travail d'arrire-plan,0 sinon

}



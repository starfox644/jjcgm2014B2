/*
 * commandIntegre.c
 *
 *  Created on: 19 janv. 2014
 *      Author: galdween
 */
#include "commandIntegre.h"



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
	free(liste_Process);
	return;
}



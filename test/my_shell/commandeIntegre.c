/*
 * commandIntegre.c
 *
 *  Created on: 19 janv. 2014
 *      Author: galdween
 */
#include "commandeIntegre.h"

/*
 * fonction permettant d'afficher les processus système en cours d'execution
 */
void afficherProcess(){
	int *liste_Process;
	int nbProcessus = GetNbProcess(), i = 0 ;
	int compteur = 0;
	//on alloue la mémoire et on recupere la liste des processus
	liste_Process = (int*) malloc(nbProcessus * (2 * sizeof(int)));
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
	//liberation de la mémoire
	free(liste_Process);
	return;
}

/*
 * fonction permettant d'afficher le contenu du dossier courant
 */
void do_ls();

/*
 * Fonction permettant d'afficher le chemin local
 */
void do_pwd();

/*
 * fonction permettant de copier un fichier Unix vers un fichier NachOS
 */
void do_copie(char* source , char* destination);

/*
 * fonction permettant de supprimer le fichier passé en paramètre du système NachOS
 */
void do_remove(char* element);
/*
 * fonction permettant d'afficher l'intégralité du contenu du systeme de fichier
 */
void afficherSysteme();
/*
 * fonction permettant de naviguer dans le système de fichier
 */
void do_cd(char* destination);

/*
 * fonction permettant d'afficher le manuel d'aide des commandes
 */
void afficherManuel(char* element){
	char quit;
	char *extension = ".txt";
	int fileDescripteur;
	element = realloc(element, StrLengh(element) + StrLengh(extension));
	StrCat(extension,element);
	//ouverture du fichier
	fileDescripteur = OpenFile(element, SEEK_SET, O_RDONLY); // on ouvre les pages du manuel en lecture seule
	// copie du texte du fichier dans la variable texte
	while((quit = GetChar()) != 'q'){ // temps qu'on ne tape pas Q on affiche le manuel
		//affichage du texte
		//Printf("%s",getContent(fileDescripteur);
	}
	//liberation de la mémoire
	free(element);
	return;

}


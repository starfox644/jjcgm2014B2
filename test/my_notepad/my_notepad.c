/*
 * my_notepad.c
 * Le but de ce programme est d'avoir un éditeur de texte sous NachOS (style cat >.>)
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */
#include "my_notepad.h"

int main(/*int argc, char* argv[]*/){
	char selection;
	char filename[10];
	//if(argc == 0){// si le programme est lancé sans un fichier on affiche le menu
		while(1){
			afficherMenu();
			selection = GetChar();
			switch (selection){
				case '1':// creation du fichier
					creerFichier();
					break;
				case '2' :// ouverture d'un fichier existant
					Printf("Quel fichier voulez vous ouvrir ?\n",0);
					Scanf("%s",filename);
					ouvrirFichier(filename);
					break;
				case '3' : //on quitte l'application
					Printf("Au Revoir\n",0);
					Exit(0);
					break;
				default : // au cas ou on rentre une commande qui n'est pas dans le menu
					Printf("Commande non reconnu\n",0);
					Printf("Fonctionnement : my_notepad <FILE>...\n",0);
					break;
			}
		}
		return 0;
	/*}else{ // si le programme est lancé avec un fichier alors on l'affiche

		// si le fichier existe on l'ouvre
		if(ouvrirFichier(argv[1]) == -1){
			creerFichier(argv[1]);//si le fichier n'existe pas on le crée et on l'ouvre.
		}


	}*/
	//return 0;
}

//fonction qui affiche le menu de notre programme
void afficherMenu(){
	Printf("\t\tBienvenue sur le notepad de NachOS\n",0);
	Printf("Menu : \n",0);
	Printf("\t\t - Pour créer un nouveau fichier Taper 1\n",0);
	Printf("\t\t - Pour ouvrir un fichier existant taper 2\n",0);
	Printf("\t\t - Pour quitter taper 3\n",0);
	return;
}

int creerFichier(){
	return 0;
}
int ouvrirFichier(char filename){

	return 0;
}


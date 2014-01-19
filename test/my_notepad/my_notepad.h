/*
 * my_notepad.h
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef MY_NOTEPAD_H_
#define MY_NOTEPAD_H_
#include "../nachos_libc/String.h"
#include "../nachos_libc/Printf.h"
#include "../nachos_libc/File.h"
#include "../nachos_libc/util.h"
#include "syscall.h"

//on définie une limite au nombre de caractère que nous pouvons entrée dans un fichier
//pour éviter des problèmes d'espace mémoire qui son limité pour le moment

#define MAX_CHAR (2048)

//définition de notre buffer
char Buffer[MAX_CHAR];
/*
 * Fonction permettant de créer un fichier
 * Retourne -1 sir la création à échoué
 * Sinon retourne le file descripteur
 */
int creerFichier();
 /*
  * Fonction permettant d'ouvrir un fichier existant
  * Retourne -1 si l'ouverture ne marche pas.
  * Sinon retourne le file Descripteur
  */
int ouvrirFichier(char*);
/*
 * Fonction servant a afficher le menu de notre notepad
 *
 */
void afficherMenu();

#endif /* MY_NOTEPAD_H_ */

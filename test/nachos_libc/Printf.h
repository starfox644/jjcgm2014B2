/*
 * Printf.h
 * Fonction permettant a un utilisateur d'afficher un message sur la console en insérant une variable dans le texte.
 * %d => integer
 * %c => char
 * %s => char*
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef PRINTF_H_
#define PRINTF_H_
#include "syscall.h"
#include "String.h"
#include "util.h"
#define MAX_LENGTH (256)


/*
 * Fonction permettant d'afficher un message sur la console avec une variable
 *  %d et %i affiche un entier
 *  %c affiche un char
 *  %s afficher un string
 *  Renvoie 0 si tout va bien, -1 sinon
 */
int Printf(char* messageVoulu, ...);

/*
 * Fonction permettant de récupérer une valeur de variable écris sur la console
 *  %d et %i recupere un entier
 *  %c recupere un char
 *  %s recupere un string
 * Renvoie 0 si tout va bien, -1 sinon
 */
int Scanf(char* typeVariable, ...);



#endif /* PRINTF_H_ */

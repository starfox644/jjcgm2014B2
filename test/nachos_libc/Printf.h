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
#define MAX_LENGH (256)


/*
 * Fonction permettant d'afficher un message sur la console avec une variable
 *  %d et %i affiche un entier
 *  %c affiche un char
 *  %s afficher un string
 */
void Printf(char* messageVoulu, ...);

/*
 * Fonction permettant de récupérer une valeur de variable écris sur la console
 */
void* Scanf(char* typeVariable, void * variable);



#endif /* PRINTF_H_ */

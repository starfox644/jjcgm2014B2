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
#define MAX_LENGH 256


void Printf(char* messageVoulu, void* variable);



#endif /* PRINTF_H_ */

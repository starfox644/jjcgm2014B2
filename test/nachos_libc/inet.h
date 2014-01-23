/*
 * inet.h
 *
 *  Created on: 23 janv. 2014
 *      Author: galdween
 */

#ifndef INET_H_
#define INET_H_

#include "syscall.h"

#define MAX_SOCKET 2;

//variable
socket id; //identifiant de notre socket unique a chaque fois MAX_INT en limite

//fonction

int connexion(int,int,char*);
int deconnexion(int);



#endif /* INET_H_ */

/*
 * inet.h
 *
 *  Created on: 23 janv. 2014
 *      Author: galdween
 */
#ifdef CHANGED
#ifndef INET_H_
#define INET_H_

#include "syscall.h"

#define MAX_SOCKET 2;

//variable
sock_t socket; //identifiant de notre socket unique a chaque fois, MAX_INT en limite

//fonction
/*
 * Fonction permettant d'initialiser la socket
 * Retourne l'identifiant de la socket
 * -1 si jamais cela echoue
 */
sock_t connexion(int,int,char*);
/*
 * permet de fermer la socket
 * retourne -1 si cela echoue
 */
int deconnexion(sock_t);





#endif /* INET_H_ */
#endif

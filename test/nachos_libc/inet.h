/*
 * inet.h
 *
 *  Created on: 23 janv. 2014
 *      Author: galdween
 */
#ifndef INET_H_
#define INET_H_

#include "syscall.h"
#include "../nachos_libc/Printf.h"
//variable

//fonction
/*
 * Fonction permettant d'initialiser la socket
 * Retourne l'identifiant de la socket
 * -1 si jamais cela echoue
 *
 * A besoin du numero de mailBox
 * le numéro de la cible
 * le numéro de l'hote
 * et la taille du buffer
 */
sock_t creerSocket(int,int,int,char*);
/*
 * permet de fermer la socket
 * retourne -1 si cela echoue
 */
int fermerSocket(sock_t);
/*
 * permet d'envoyer un message
 * retourne -1 si cela echoue
 */
int envoyerMessage(sock_t,char*);
/*
 * permet de recevoir un message
 * retourne -1 si cela echoue
 */
int recevoirMessage(sock_t,char*);

#endif /* INET_H_ */

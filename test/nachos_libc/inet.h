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
//variable
sock_t socket; //identifiant de notre socket unique a chaque fois, MAX_INT en limite

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
sock_t InitSocket(int,int,int,char*);
/*
 * permet de fermer la socket
 * retourne -1 si cela echoue
 */
int CloseSocket(sock_t);
/*
 * permet d'envoyer un message
 * retourne -1 si cela echoue
 */
int Send(sock_t,char*);
/*
 * permet de recevoir un message
 * retourne -1 si cela echoue
 */
int Receive(sock_t,char*);

#endif /* INET_H_ */
#endif
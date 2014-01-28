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
 * A besoin de l'adresse de la cible
 */
sock_t creerSocket(int adr);

/*
 * permet de fermer la socket
 * retourne -1 si cela echoue
 */
int fermerSocket(sock_t socket); // TODO voir pourquoi ca plante

/*
 * permet d'envoyer un message
 * retourne -1 si cela echoue
 */
int envoyerMessage(sock_t socket, char* messageEnvoi);

/*
 * permet de recevoir un message
 * retourne -1 si cela echoue
 */
int recevoirMessage(sock_t socket,char* messageReception);

#endif /* INET_H_ */

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

//fonctioner
/*
 * Fonction permettant d'initialiser la socket
 * Retourne l'identifiant de la socket
 * -1 si jamais cela echoue
 *
 * A besoin de l'adresse de la cible
 */
sock_t creerSocket(int adr);

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

/*
 * permet d'envoyer un message en étant sur qu'il soit bien arrivé
 * ou alors affiche un message pour dire qu'il n'est pas arrivé
 */
int envoyerMessageSecurise(sock_t,char*);

/*
 * permet de recevoir un message et d'envoyer une confirmation de la reception
 */
int recevoirMessageSecurise(sock_t,char*);

#endif /* INET_H_ */

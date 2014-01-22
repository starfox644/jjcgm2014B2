/*
 * inet.c
 *
 *  Created on: 23 janv. 2014
 *      Author: galdween
 */
#include "inet.h"

sock_t creerSocket(int farAddr){
	//identifiant de notre socket unique a chaque fois, MAX_INT en limite
	sock_t socket;
	//envoie de l'appel systeme pour la création du socket
	socket = InitSocket(farAddr);
	//on renvoie la socket creee ou -1 si cela a echoue
	return socket;
}


int fermerSocket(sock_t socketUser){
	int erreur = 0;
	//envoi de l'appel système pour la fermeture du socket
	erreur = CloseSocket(socketUser);
	//on renvoie -1 si cela a echoue ou 0 sinon
	return erreur;
}

int envoyerMessage(sock_t socketUser,char* message){
	int nbChar = 0;
	//envoi de l'appel système pour envoyer le message
	nbChar = Send(socketUser, message);
	//on retourne le nombre de char envoyes et -1 si cela echoue
	return nbChar;
}
int recevoirMessage(sock_t socketUser,char* message){
	int nbChar = 0;
	//envoi de l'appel système pour recevoir le message
	nbChar = Receive(socketUser, message);
	//on renvoie le nombre de char reçu et -1 si cela a échoué
	return nbChar;
}



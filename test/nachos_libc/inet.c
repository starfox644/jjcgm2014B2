/*
 * inet.c
 *
 *  Created on: 23 janv. 2014
 *      Author: galdween
 */
#ifdef CHANGED
#include "inet.h"

sock_t connexion(int numBox, int to ,int from ,char* buffer){

	//envoir de l'appel systeme pour la création du socket
	socket = SockInit(to,from, buffer);
	//on renvoi la socket crée/-1 si cela à échoué
	return socket;
}


int deconnexion(sock_t socketUser){
	int erreur = 0;
	//envoi de l'appel système pour la fermeture du socket
	erreur = SockClose(socketUser);
	//on renvoi -1 si cela à échoué ou 0 sinon
	return erreur;
}

int envoiMessage(sock_t socketUser,char* message){
	int nbChar;
	//envoi de l'appel système pour envoyé le message
	nbChar = Send(socketUser, message);
	//on retourne le nombre de char envoyé et -1 si cela échoue
	return nbChar;
}
int recevoirMessage(sock_t socketUser,char* message){
	int nbChar;
	//envoi de l'appel système pour recevoir le message
	nbChar = Receive(socketUser, message);
	//on renvoi le nombre de char reçu et -1 si cela à échoué
	return nbChar;
}

#endif

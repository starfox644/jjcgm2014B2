/*
 * socket.h
 *
 *  Created on: 24 janv. 2014
 *      Author: Sarkal
 */
#ifdef NETWORK
#ifndef __SOCKET__H__
#define __SOCKET__H__

#include "synch.h"

/*
 * Demande l'initialisation de la socket et renvoie l'id attribuee.
 * Renvoie -1 si erreur.
 */
int do_SockInit(int farAddr);

class Socket {

public :
	/**
	 * Cree la socket entre la machine actuelle et la machine distante
	 * d'adresse newfarAddr
	 */
	Socket (int id, int farAddr);

	~Socket();

	/**
	 * Envoie le message au destinataire de la socket
	 * Renvoie le nombre de caracteres envoyes.
	 */
	int do_SendSocket(char* message);

	/**
	 * Attend la reception d'un message et le copie dans la memoire
	 * de la machine MIPS.
	 * Renvoie le nombre de caracteres ecrits
	 * Renvoie -1 si l'ecriture a echoue
	 */
	int do_ReceiveSocket(int adrMessage);

	// Getters
	int getId()		{ return id; }
	char* getName()	{ return name; }

	char* buffer;
	// Pour verrouiller l'acces a la socket
	Semaphore *semSocket;

private:
	int id;
	int farAddr;
	char *name;

};
#endif // __SOCKET__H__
#endif // NETWORK

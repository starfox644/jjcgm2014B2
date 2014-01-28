/*
 * socket.h
 *
 *  Created on: 23 janv. 2014
 *      Author: Sarkal
 */
#ifdef NETWORK
#ifndef __SOCKET__H__
#define __SOCKET__H__

#include "synch.h"


int do_SockInit(int farAddr);

class Socket {

public :
	// Constructeur
	Socket (int id, int farAddr);
	~Socket();
	// Demande l'envoi d'un message
	int do_SendSocket(char* message);
	// Demande la reception d'un message
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

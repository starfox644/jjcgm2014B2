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


int do_SockInit(int numBox, int newTo, int newFrom, int buffer);

class Socket {

public :
	// Constructeur
	Socket (int id, int numBox, int newTo, int newFrom, char* newBuffer);
	~Socket();
	// Demande l'envoi d'un message
	int do_SendSocket(char* message);
	// Demande la reception d'un message
	int do_ReceiveSocket(int adrMessage);

	// Getters
	int getId()		{ return id; }
	char* getName()	{ return name; }
	int getTo()		{ return to; }
	int getFrom()	{ return from; }

	char* buffer;
	// Pour verrouiller l'acces a la socket
	Semaphore *semSocket;

private:
	int id;
	int to;
	int from;
	int numBox;
	bool isUsed;
	char *name;

};
#endif // __SOCKET__H__
#endif // NETWORK

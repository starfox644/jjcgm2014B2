/*
 * socketManager.h
 *
 *  Created on: 24 janv. 2014
 *      Author: Sarkal
 */
#ifdef NETWORK
#ifndef __SOCKETMANAGER__H__
#define __SOCKETMANAGER__H__

#include "synch.h"
#include "socket.h"
#include "../network/post.h"



class SocketManager {


public :
	SocketManager();
	~SocketManager();

	std::list<Socket*> socketList;

	/**
	 * Ajoute la socket a la liste
	 * Renvoie -1 si nextSocketId a atteint INT_MAX
	 */
	int addSocket(int farAddr);

	/**
	 * Supprime la socket id de la liste.
	 * Si id n'est pas valide, la socket est detruite
	 * Sinon, renvoie -1
	 */
	int removeSocket(int id);

	/**
	 * Renvoie la socket identifiee par id
	 * Renvoie NULL si celle-ci n'est pas dans la liste
	 */
	Socket* getSocket(int id);

	/**
	 * Utilise la socket id pour envoyer le message a adrMessage
	 * Renvoie le nombre de caracteres ecrits
	 * Renvoie -1 si : - Le message est long de plus de MAX_STRING_SIZE caracteres
	 * 				   - Id n'est pas valide
	 */
	int do_Send(int id, int adrMessage);

	/**
	 * Demande la reception d'un message a la socket idSocket
	 * Celui-ci est ecrit a adrMessage
	 * Renvoie le nombre de caracteres lus.
	 * Renvoie -1 si id n'est pas valide
	 */
	int do_Receive(int id, int adrMessage);

	/**
	 * Supprime la liste de sockets
	 */
	void deleteSocketList();

private :
	// nombre de sockets creees
	int nbSocket;
	int nextSocketId;

};
#endif // __SOCKETMANAGER__H__
#endif // NETWORK

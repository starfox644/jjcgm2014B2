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

	// Socket list : needed to give an unique identifier for user sockets
	std::list<Socket*> socketList;
	/**
	 * Add newSem to semList and allocate it a unique modifier
	 */
	int addSocket(int farAddr, int to, int from, char* buffer);

	/**
	 * Remove a semaphore from the list based on his identifier.
	 * If the identifier is valid, the semaphore is destroyed.
	 * If not, the function returns -1.
	 */
	int removeSocket(int id);

	/**
	 * Return the semaphore identified by id, or NULL if it doesn't exist
	 */
	Socket* getSocket(int id);

	/**
	 * Send the message at adrMessage via the socket idSocket
	 * Return the number of written characters
	 */
	int do_Send(int idSocket, int adrMessage);

	/**
	 * Receive a message via the socket idSocket and write it at adrMessage
	 * Return the number of read characters
	 */
	int do_Receive(int idSocket, int adrMessage);
	/**
	 * Delete the semaphore list
	 */
	void deleteSocketList();

private :
	// number of socket created
	int nbSocket;
	int nextSocketId;

};
#endif // __SOCKETMANAGER__H__
#endif // NETWORK

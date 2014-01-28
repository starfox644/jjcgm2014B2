
#ifdef NETWORK
#include "socketManager.h"
#include "syscall.h"
#include "system.h"


SocketManager::SocketManager()
{
	nbSocket = 0;
	nextSocketId = 0;
}

SocketManager::~SocketManager()
{
	deleteSocketList();
}

/**
 * Add a new socket to the list.
 * Return -1 if ID has reached INT_MAX
 */
int SocketManager::addSocket(int farAddr)
{
	if (nextSocketId < INT_MAX)
	{
		Socket *socket = new Socket(nextSocketId, farAddr);
		nextSocketId++;
		nbSocket++;
		socketList.push_back(socket);
		return socket->getId();
	}
	else
		return -1;
}

/**
 * Remove a socket from the list based on its identifier.
 * If the identifier is valid, the socket is destroyed.
 * If not, the function returns -1.
 */
int SocketManager::removeSocket(int id)
{
	// iterator to find the socket in the list
	std::list<Socket*>::iterator it=socketList.begin();
	while (it != socketList.end() && (*it)->getId() != id)
		it++;
	// If socket not found, return -1 : error
	if ((*it)->getId() != id)
		return -1;
	// Else, remove it
	else
	{
		// On attend que la socket soit libre
		(*it)->semSocket->P();
		nbSocket--;
		(*it)->semSocket->V();
		delete *it;
		socketList.erase(it);
		return 0;
	}
}

/**
 * Return the socket identified by id, or NULL if it doesn't exist
 */
Socket* SocketManager::getSocket(int id)
{
	// iterator to find the semaphore in the list
	std::list<Socket*>::iterator it=socketList.begin();
	while (it != socketList.end() && (*it)->getId() != id)
		it++;
	if(it == socketList.end())
		return NULL;
	// If socket not found, return -1 : error
	if ((*it)->getId() != id)
		return NULL;
	// Else, return it
	else
		return (Socket*)(*it);
}

/**
 * Send the message at adrMessage via the socket idSocket
 * Return the number of written characters
 */
int SocketManager::do_Send(int id, int adrMessage)
{
	char message[MAX_STRING_SIZE];
	int c;
	int i = 0, nbChar = 0;

	// On recupere le message char par char
	machine->ReadMem(adrMessage, 1, &c);
	while (i < MAX_STRING_SIZE && c != '\0')
	{
		message[i] = (char) c;
		i++;
		machine->ReadMem(adrMessage+i, 1, &c);
	}

	// Si le message est plus long que MAX_STRING_SIZE, erreur car il n'y aura pas de '\0'
	if (i == MAX_STRING_SIZE)
		return -1;
	// sinon on rajoute un '\0'
	else
		message[i] = '\0';

	// Iterator to find the socket
	std::list<Socket*>::iterator it=socketList.begin();
	while (it != socketList.end() && (*it)->getId() != id)
		it++;
	// If socket not found, return -1 : error
	if ((*it)->getId() != id)
		nbChar = -1;
	// Else we can send the message
	else
		nbChar = (*it)->do_SendSocket(message);
	return nbChar;
}

/**
 * Receive a message via the socket idSocket and write it at adrMessage
 * Return the number of read characters
 */
int SocketManager::do_Receive(int idSocket, int adrMessage)
{
	int nbChar = 0;
	// Iterator to find the socket
	std::list<Socket*>::iterator it=socketList.begin();
	while (it != socketList.end() && (*it)->getId() != idSocket)
		it++;
	// If socket not found, return -1 : error
	if ((*it)->getId() != idSocket)
		nbChar = -1;
	else
		nbChar = (*it)->do_ReceiveSocket(adrMessage);
	return nbChar;
}

/**
 * Delete the socket list
 */
void SocketManager::deleteSocketList()
{
	std::list<Socket*>::iterator it;
	for (it = socketList.begin() ; it != socketList.end() ; it++)
		delete *it;
}

#endif // NETWORK

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
 * Ajoute la socket a la liste
 * Renvoie -1 si nextSocketId a atteint INT_MAX
 */
int SocketManager::addSocket(int farAddr)
{
	if (nextSocketId < INT_MAX)
	{
		Socket *socket = new Socket(nextSocketId, farAddr);
		nextSocketId++;
		nbSocket++;
		// Ajoute la socket a la liste
		socketList.push_back(socket);
		return socket->getId();
	}
	else
		return -1;
}

/**
 * Supprime la socket id de la liste.
 * Si id n'est pas valide, la socket est detruite
 * Sinon, renvoie -1
 */
int SocketManager::removeSocket(int id)
{
	// iterateur pour trouver la socket dans la liste
	std::list<Socket*>::iterator it=socketList.begin();
	while (it != socketList.end() && (*it)->getId() != id)
		it++;
	// Si la socket n'est pas trouve
	if ((*it)->getId() != id)
		return -1;
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
 * Renvoie la socket identifiee par id
 * Renvoie NULL si celle-ci n'est pas dans la liste
 */
Socket* SocketManager::getSocket(int id)
{
	// iterateur permettant de trouver la socket
	std::list<Socket*>::iterator it=socketList.begin();
	while (it != socketList.end() && (*it)->getId() != id)
		it++;

	// Si on ne la trouve pas, renvoie NULL
	if(it == socketList.end() || (*it)->getId() != id)
		return NULL;
	else
		return (Socket*)(*it);
}

/**
 * Utilise la socket id pour envoyer le message a adrMessage
 * Renvoie le nombre de caracteres ecrits
 * Renvoie -1 si : - Le message est long de plus de MAX_STRING_SIZE caracteres
 * 				   - Id n'est pas valide
 */
int SocketManager::do_Send(int id, int adrMessage)
{
	char message[MAX_STRING_SIZE];
	int c;
	int i = 0, nbChar = 0;
	Socket *socket;

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

	// Si on ne trouve pas la socket : renvoie -1
	if ((socket = getSocket(id)) == NULL)
		nbChar = -1;
	else
		nbChar = socket->do_SendSocket(message);
	return nbChar;
}

/**
 * Demande la reception d'un message a la socket idSocket
 * Celui-ci est ecrit a adrMessage
 * Renvoie le nombre de caracteres lus.
 * Renvoie -1 si id n'est pas valide
 */
int SocketManager::do_Receive(int id, int adrMessage)
{
	int nbChar = 0;
	Socket *socket;
	// Si on ne trouve pas la socket : renvoie -1
	if ((socket = getSocket(id)) == NULL)
		nbChar = -1;
	else
		nbChar = socket->do_ReceiveSocket(adrMessage);
	return nbChar;
}

/**
 * Supprime la liste de sockets
 */
void SocketManager::deleteSocketList()
{
	std::list<Socket*>::iterator it;
	for (it = socketList.begin() ; it != socketList.end() ; it++)
		delete *it;
}

#endif // NETWORK

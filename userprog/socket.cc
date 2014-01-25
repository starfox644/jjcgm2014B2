#ifdef NETWORK
/*
 * socket.cc
 *
 *  Created on: 24 janv. 2014
 *      Author: Sarkal
 */

#include "system.h"
#include "socket.h"
#include <list>

/*
 * Initialise la socket et renvoie l'id attribuee.
 * Renvoie -1 si erreur.
 */
int do_SockInit(int numBox, int to, int from, int adrBuffer)
{
	return currentProcess->socketManager->addSocket(numBox, to, from, (char *) adrBuffer);
}

Socket::Socket(int newId, int newNumBox, int newTo, int newFrom, char* newBuffer)
{
	id = newId;
	numBox = newNumBox;
	to = newTo;
	from = newFrom;
	buffer = newBuffer;
}

Socket::~Socket()
{
	delete buffer;
}

/**
 * Envoie le message au destinataire de la socket
 * Renvoie le nombre de caracteres envoyes.
 * Renvoie -1 : - si le message lu n'est pas valable
 * 				- si la socket idSocket n'existe pas
 */
int Socket::do_SendSocket(char *message)
{
	PacketHeader outPktHdr;
	MailHeader outMailHdr;
	// construct packet, mail header for original message
	// To: destination machine, mailbox 0
	// From: our machine, reply to: mailbox 1
	outPktHdr.to = numBox;
	outMailHdr.to = to;
	outMailHdr.from = from;
	outMailHdr.length = strlen(message) + 1;
	// Send the first message
	postOffice->Send(outPktHdr, outMailHdr, message);
	return strlen(message);
}

int Socket::do_ReceiveSocket(int adrMessage)
{
	PacketHeader inPktHdr;
	MailHeader inMailHdr;
	// Wait for the first message from the other machine
	postOffice->Receive(numBox, &inPktHdr, &inMailHdr, buffer);

	// On ecrit le message char par char
	int i = 0;
	bool isSuccess = true;
	while (i < MAX_STRING_SIZE && buffer[i] != '\0')
	{
		isSuccess = machine->WriteMem(adrMessage+i, 1, buffer[i]);
		i++;
	}
	if (i == MAX_STRING_SIZE && buffer[i-1] != '\0')
		buffer[i-1] = '\0';
	else if (!isSuccess)
		return -1;

	return strlen(buffer);
}

#endif // NETWORK

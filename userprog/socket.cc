#ifdef NETWORK
#ifdef CHANGED
/*
 * socket.cc : Reprise de nettest pour l'envoi et la reception
 * 	de message via sockets
 *
 *  Created on: 24 janv. 2014
 *      Author: Sarkal
 */

#include "system.h"
#include "socket.h"
#include <list>

/*
 * Demande l'initialisation de la socket et renvoie l'id attribuee.
 * Renvoie -1 si erreur.
 */
int do_SockInit(int farAddr)
{
	int id = currentProcess->socketManager->addSocket(farAddr);
	return id;
}

/**
 * Cree la socket entre la machine actuelle et la machine distante
 * d'adresse newfarAddr
 */
Socket::Socket(int newId, int newfarAddr)
{
	id = newId;
	farAddr = newfarAddr;
	buffer = new char[256];
}

Socket::~Socket()
{
	delete buffer;
}

/**
 * Envoie le message au destinataire de la socket
 * Renvoie le nombre de caracteres envoyes.
 */
int Socket::do_SendSocket(char *message)
{
//	printf("do_SendSocket, %s\n", message);
	PacketHeader outPktHdr;
	MailHeader outMailHdr;
	// construct packet, mail header for original message
	// To: destination machine, mailbox 0
	// From: our machine, reply to: mailbox 1
	outPktHdr.to = farAddr;
	outPktHdr.from = postOffice->getNetAddr();
	outMailHdr.to = 0;
	outMailHdr.from = 1;
	outMailHdr.length = strlen(message) + 1;
	// Envoie le message
	postOffice->Send(outPktHdr, outMailHdr, message);
	return strlen(message);
}

/**
 * Attend la reception d'un message et le copie dans la memoire
 * de la machine MIPS.
 * Renvoie le nombre de caracteres ecrits
 * Renvoie -1 si l'ecriture a echoue
 */
int Socket::do_ReceiveSocket(int adrMessage)
{
	PacketHeader inPktHdr;
	MailHeader inMailHdr;

	// On attend la reception d'un message
	postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);

	// On ecrit le message char par char
	int i = 0;
	bool isSuccess = true;
	while (i < MAX_STRING_SIZE && isSuccess && buffer[i] != '\0')
	{
		isSuccess = machine->WriteMem(adrMessage+i, 1, buffer[i]);
		i++;
	}
	// Si le message est plus long que MAX_STRING_SIZE, on le coupe
	if (i == MAX_STRING_SIZE && buffer[i-1] != '\0')
		buffer[i-1] = '\0';
	else if (!isSuccess)
		return -1;
	return strlen(buffer);
}
#endif // CHANGED
#endif // NETWORK

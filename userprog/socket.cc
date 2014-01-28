#ifdef NETWORK
/*
 * socket.cc : Reprise des fonctions de nettest
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
int do_SockInit(int farAddr)
{
	int id = currentProcess->socketManager->addSocket(farAddr);
	return id;
}

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
 * Renvoie -1 : - si le message lu n'est pas valable
 * 				- si la socket idSocket n'existe pas
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
	// Send the first message
	postOffice->Send(outPktHdr, outMailHdr, message);
	return strlen(message);
}

int Socket::do_ReceiveSocket(int adrMessage)
{
	PacketHeader inPktHdr;
	MailHeader inMailHdr;
//	printf("[Socket::do_ReceiveSocket] Debut fonction\n");
	// Wait for the first message from the other machine
	postOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
//	printf("[Socket::do_ReceiveSocket] apres post->receive\n");
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
//	printf("[Socket receive] buffer : %s", buffer);
	return strlen(buffer);
}

#endif // NETWORK

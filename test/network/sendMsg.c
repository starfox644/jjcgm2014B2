/*
 * inetAnneau.c
 *	programme de test pour envoyer un message sur un anneau de machine
 *	chaque machine va etre representé par un thread.
 *  Created on: 19 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#include "../nachos_libc/inet.h"
#include "../nachos_libc/Printf.h"
#include "../nachos_libc/String.h"
#include "../nachos_libc/util.h"
//programme principal
int main(){

	Printf("\n-----------------------------------------\n");
	Printf("A lancer avec -m 2 -o 1 et getMsg sur une autre machine\n");
	Printf("Lancement du test sendMsg : \n");
	Printf("Envoie un message a une machine et attend l'ack\n");
	Printf("-----------------------------------------\n");

	char* message = (char*) malloc(10 * sizeof(char));
	char* reception = (char*) malloc(10 * sizeof(char));
	sock_t Socket;

	// Attente connexion d'une autre machine
	Sleep(5);

	Socket = creerSocket(1);

	// Envoie
	message = "Hello";
	if(envoyerMessage(Socket,message) ==-1){
		Printf("Erreur d'envoi de Message\n");
	}

	if(recevoirMessage(Socket,reception) == -1){
		Printf("Erreur de réception de message\n");
	}

	Printf("Message reçu : %s\n",reception);


	free(reception);
	free(message);
	return 0;
}

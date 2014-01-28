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
	Printf("Lancement du test sendMsg : \n");
	Printf("Envoie un message a une machine et attend l'ack\n");
	Printf("-----------------------------------------\n");
//	int to = 1, to_numBox, from_numBox;
	char* message = (char*) malloc(10 * sizeof(char));
	char* reception = (char*) malloc(10 * sizeof(char));
	sock_t SocketEnvoi, SocketRecep;

	// Attente connexion d'une autre machine
	Sleep(5);

	SocketEnvoi = creerSocket(1);

	// Envoie
	message = "Hello";
	if(envoyerMessage(SocketEnvoi,message) ==-1){
		Printf("Erreur d'envoi de Message\n");
	}

	SocketRecep = creerSocket(1);
	if(recevoirMessage(SocketRecep,reception) == -1){
		Printf("Erreur de réception de message\n");
	}

	Printf("Message reçu : %s\n",reception);


	//on a fini on se déconnecte
//	fermerSocket(Socket);
	free(reception);
	free(message);
	Printf("Fin du main\n");
	return 0;
}

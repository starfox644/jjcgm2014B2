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
	Printf("Lancement du test getMsg : \n");
	Printf("Attend un message d'une machine et lui renvoie un ack\n");
	Printf("-----------------------------------------\n");
//	int to = 0,from = 2;
	char* message = (char*) malloc(10 * sizeof(char));
	char* reception = (char*) malloc(10 * sizeof(char));
	sock_t SocketEnvoi, SocketRecep;

	// Attente connexion d'une autre machine
	Sleep(5);

	SocketRecep = creerSocket(2);
	if(recevoirMessage(SocketRecep,reception) == -1)
		Printf("Erreur de réception de message\n");

	Printf("Message recu : %s\n", reception);

	SocketEnvoi = creerSocket(2);
	//si on est la première machine on envoi le message
	message = "Got it"; //on donne le numero de la machine de départ

	if(envoyerMessage(SocketEnvoi,message) ==-1)
		Printf("Erreur d'envoi de Message\n");

	//on a fini on se déconnecte
//	fermerSocket(Socket);
//	free(reception);
//	free(message);
	Printf("Fin du main\n");
	return 0;
}

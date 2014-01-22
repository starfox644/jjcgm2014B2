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
	Printf("Lancement du test inetAnneau : \n");
	Printf("envoi un message a une machine qui l'envoi a la suivante\n");
	Printf("-----------------------------------------\n");
	int to = 0,from = 1;
	char* message = (char*) malloc(10 * sizeof(char));
	char* reception = (char*) malloc(10 * sizeof(char));
	sock_t SocketEnvoi, SocketReception;

	//demande a l'utilisateur les informations pour créer la socket
	Printf("FROM : \n");
	Scanf("%d",&from);
	Printf("TO :\n");
	Scanf("%d",&to);
	Sleep(5);
	//connexion de notre machine
	SocketEnvoi = creerSocket(to, 0, 1, message);
	SocketReception = creerSocket(to, 1, 0, reception);
	//si on est la première machine on envoi le message


	if(GetHostname() == 0){
		message = Itoa(from); //on donne le numero de la machine de départ
		if(envoyerMessage(SocketEnvoi,message) ==-1){
			Printf("Erreur d'envoi de Message depuis la machine %d\n",from);
		}

		if(recevoirMessage(SocketReception,reception) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
		}
	}else{
		if(recevoirMessage(SocketReception,reception) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
		}
		StrCpy(reception,message);
		if(envoyerMessage(SocketEnvoi,message) ==-1){
			Printf("Erreur d'envoi de Message depuis la machine %d\n",from);
		}
	}
	if(Atoi(reception) == from){	//si le message recu correspond au numéro d'hote alors on a fait la boucle
		Printf("\nMessage reçu : %s\n",reception);
		Printf("\nboucle faite\n");
	}else{
		Printf("\nMessage reçu : %s\n",reception);
	}


	//on a fini on se déconnecte
	free(reception);
	free(message);
	Printf("Fin du main\n");
	return 0;
}

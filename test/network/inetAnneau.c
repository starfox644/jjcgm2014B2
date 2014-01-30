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
	Printf("Il faut que la premiere machine porte le numéro 0,\n");
	Printf("et la derniere machine doit envoyer un message vers la premiere machine donc 0 \n");
	Printf("Exemple machine depart : ./nachos -m 0 -o X -x ./network/inetAnneau\n");
	Printf("\t\t\t...\n");
	Printf("Exemple machine de fin : ./nachos -m X -o 0 -x ./network/inetAnneau\n");
	Printf("-----------------------------------------\n");
	int to = 0;
	char* message = (char*) malloc(10 * sizeof(char));
	char* reception = (char*) malloc(10 * sizeof(char));
	sock_t SocketEnvoi, SocketReception;

	//demande a l'utilisateur les informations pour creer la socket
	Printf("Adresse de la machine cible :\n");
	Scanf("%d",&to);
	Printf("\n");
	Sleep(5);
	//connexion de notre machine
	SocketEnvoi = creerSocket(to);
	SocketReception = creerSocket(to);
	//si on est la premiere machine on envoi le message


	if(GetHostname() == 0){
		message = Itoa(GetHostname()); //on donne le numero de la machine de depart
		Printf("[Machine #%i] Envoi du message  \"%i\" a la machine #%i\n", GetHostname(), Atoi(message), to);
		if(envoyerMessage(SocketEnvoi,message) ==-1){
			Printf("Erreur d'envoi de Message depuis la machine %d\n",GetHostname());
		}

		if(recevoirMessage(SocketReception,reception) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
		}
	}else{
		if(recevoirMessage(SocketReception,reception) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
		}
		Printf("[Machine #%i] Message reçu : %i\n", GetHostname(), Atoi(reception));
		Printf("[Machine #%i] Transmission du message \"%i\" a la machine #%i\n\n", GetHostname(), Atoi(message), to);
		StrCpy(reception,message);
		if(envoyerMessage(SocketEnvoi,message) ==-1){
			Printf("Erreur d'envoi de Message depuis la machine %d\n",GetHostname());
		}
	}
	if(Atoi(reception) == GetHostname()){	//si le message recu correspond au numero d'hote alors on a fait la boucle
		Printf("[Machine #%i] Message reçu : %i\n", GetHostname(), Atoi(reception));
		Printf("\n[Machine #%i] Le message est revenu a la machine source, on s'arrete\n\n", GetHostname());
	}

	free(reception);
	free(message);
	return 0;
}

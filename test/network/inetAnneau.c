/*
 * inetAnneau.c
 *	programme de test pour envoyer un message sur un anneau de machine
 *	chaque machine va etre representé par un thread.
 *  Created on: 19 janv. 2014
 *      Author: galdween
 */
#ifdef NETWORK
#include "syscall.h"
#include "../nachos_libc/inet.h"
#include "../nachos_libc/Printf.h"
#include "../nachos_libc/String.h"
#include "..nachos_libc/util.h"
//programme principal
int main(){

	Printf("\n-----------------------------------------\n");
	Printf("Lancement du test inetAnneau : \n");
	Printf("envoi un message a une machine qui l'envoi a la suivante\n");
	Printf("-----------------------------------------\n");
	int to = 0,from = 0, numBox = 0;
	char message[256], reception[256];
	sock_t Socket;
	//demande a l'utilisateur les informations pour créer la socket
	Printf("Numéro de la machine :\n");
	Scanf("%d",&from);
	Printf("Numéro de la machine cible : \n");
	Scanf("%d",&to);

	//connexion de notre machine
	Socket = creerSocket(numBox,to ,from,message);
	Printf("Socket : %d\n",Socket);
	//si on est la première machine on envoi le message

	message = Itoa(from); //on donne le numero de la machine de départ

	if(from == 0){
		if(envoyerMessage(Socket,message) ==-1){
			Printf("Erreur d'envoi de Message depuis la machine %d\n",from);
		}
		if(recevoirMessage(Socket,reception) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
		}
		if(Atoi(reception) == from){	//si le message recu correspond au numéro d'hote alors on a fait la boucle
			Printf("boucle faite\n");
		}else{
			Printf("Message reçu : %s\n",reception);
		}
	}else{//sinon on recois le message et apres on envois un message
		if(recevoirMessage(Socket,reception) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
		}
		Printf("Message reçu : %s\n", reception);
		if(envoyerMessage(Socket,message) == -1){
			Printf("Erreur d'envoi de message depuis la machine %d\n",from);
		}
	}

	//on a finis on ce déconnecte
	fermerSocket(Socket);

	Printf("Fin du main\n");
	return 0;
}

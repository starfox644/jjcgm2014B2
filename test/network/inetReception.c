/*
 * inetReception.c
 *
 *  Created on: 21 janv. 2014
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
	Printf("Lancement du test inetReception : \n");
	Printf("envoi un message a une machine qui l'envoi a la suivante\n");
	Printf("-----------------------------------------\n");
	int to,from, compteur = 0, nbChar = 0;
	char* reception = (char*) malloc(2 * sizeof(char));
	sock_t SocketReception;
	Printf("From : \n");
	Scanf("%d",&from);
	Printf("To : \n");
	Scanf("%d",&to);
	//connexion de notre machine
	SocketReception = creerSocket(to);
	while(compteur < 1){
		nbChar = recevoirMessage(SocketReception,reception);
		if(nbChar == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
			return -1;
		}
		compteur++;
		Printf("Compteur : %d\n",compteur);
	}
	Printf("Message reçu : %d\n",reception);

	//on a finis on ce déconnecte
	free(reception);
	//fermerSocket(SocketEnvoi);
	Printf("Fin du main\n");
	return 0;
}



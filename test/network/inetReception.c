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
	Printf(" Lancement du test inetReception : \n");
	Printf(" Reçoit trois messages de machines distantes\n");
	Printf("-----------------------------------------\n");
	int to, compteur = 0, nbChar = 0;
	char* reception = (char*) malloc(2 * sizeof(char));
	sock_t SocketReception;
	Printf("Adresse machine cible : \n");
	Scanf("%d",&to);
	//connexion de notre machine
	SocketReception = creerSocket(to);
	while(compteur < 3){
		nbChar = recevoirMessage(SocketReception,reception);
		if(nbChar == -1){
			Printf("Erreur de réception de message avec la machine %d\n",to);
			return -1;
		}
		compteur++;
		Printf("Message reçu : %s\n",reception);
		Printf("Compteur : %d\n",compteur);
	}

	//on a finis on ce déconnecte
	free(reception);
	//fermerSocket(SocketEnvoi);
	Printf("Fin du main\n");
	return 0;
}



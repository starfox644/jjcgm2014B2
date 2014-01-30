/*
 * inetEnvoi.c
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
	Printf(" Lancement du test inetEnvoi : \n");
	Printf(" Envoie un message a une machine\n");
	Printf("-----------------------------------------\n");
	int to;
	char* message = (char*) malloc(10 * sizeof(char));
	sock_t SocketEnvoi;
	Printf("Adresse machine cible : \n");
	Scanf("%d",&to);
	//connexion de notre machine
	SocketEnvoi = creerSocket(to);

	// On demande le message a envoyer
	Printf("Entrer une chaine de caracteres a envoyer :\n");
	Scanf("%s", message);

	if(envoyerMessage(SocketEnvoi,message) ==-1){
		Printf("Erreur d'envoi de Message depuis la machine %d\n",GetHostname());
	}
 	Printf("Message envoye : %s\n",message);
	free(message);
	return 0;
}

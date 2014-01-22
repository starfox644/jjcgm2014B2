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
	Printf("Lancement du test inetEnvoi : \n");
	Printf("envoi un message a une machine\n");
	Printf("-----------------------------------------\n");
	int to,from;
	char* message = (char*) malloc(10 * sizeof(char));
	sock_t SocketEnvoi;
	Printf("From : \n");
	Scanf("%d",&from);
	Printf("To : \n");
	Scanf("%d",&to);
	//connexion de notre machine
	SocketEnvoi = creerSocket(to);

	//si on est la première machine on envoi le message
	message = Itoa(from); //on donne le numero de la machine de départ

	if(envoyerMessage(SocketEnvoi,message) ==-1){
		Printf("Erreur d'envoi de Message depuis la machine %d\n",from);
	}
 	Printf("Message envoye : %s\n",message);
	//on a finis on ce déconnecte
	free(message);
	//fermerSocket(SocketEnvoi);
	Printf("Fin du main\n");
	return 0;
}

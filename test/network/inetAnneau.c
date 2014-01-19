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

//nombre de machine dans notre anneau
#define NB_MACHINE 2

//creation de notre structure pour l'envoi des données pour nos threads
typedef struct threadstruct{
	int idMachine;
	int idContact;
	char message[256];
}ts;

//fonction de nos threads
void machine(void * arg){
	sock_t Socket;
	ts *s = (ts*) arg;
	int numBox = 1;
	//connexion de notre machine
	Socket = initSocket(numBox,s->idMachine+1 ,s->idMachine,s->message);
	//si on est la première machin on envoi le message
	if(s->idMachine == 0){
		//on crée notre message et on rajoute l'id de la machine
		StrCpy("bonjour",s->message);
		StrCat(s->message,Itoa(s->idMachine));

		if(envoiMessage(Socket,s->message) ==-1){
			Printf("Erreur d'envoi de Message depuis la machine %d\n",s->idMachine);
		}
		if(recevoirMessage(Socket,s->message) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",s->idMachine);
		}

	}else{//sinon on recois le message et apres on envois un message
		if(recevoirMessage(Socket,s->message) == -1){
			Printf("Erreur de réception de message avec la machine %d\n",s->idMachine);
		}
		//on rajoute l'id de la machine a notre message
		StrCat(s->message,Itoa(s->idMachine));
		if(envoiMessage(Socket,s->message) == -1){
			Printf("Erreur d'envoi de message depuis la machine %d\n",s->idMachine);
		}
	}

	//on a finis on ce déconnecte
	deleteSocket(Socket);
}


//programme principal
int main(){
	Printf("\n-----------------------------------------\n");
	Printf("Lancement du test inetAnneau : \n");
	Printf("Cree plusieurs machine qui ce passe un message qui doit faire le tour de l'anneau\n");
	Printf("-----------------------------------------\n");
	int numMachine = 0;
	int tid[NB_MACHINE];
	ts *t = 0;

	for(numMachine = 0; numMachine < NB_MACHINE; numMachine++){
		t->idContact = numMachine+1;
		t->idMachine = numMachine;
		tid[numMachine] = UserThreadCreate(machine,t);
	}
	Printf("Fin du main\n");
	return 0;
}


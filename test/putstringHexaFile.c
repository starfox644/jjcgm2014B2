/*
 * putStringHexa0xFF.c
 *
 *  Created on: 8 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"

int main()
{
	int MessageErreur; //variable de recuperation du message d'erreur
	OpenFileId input = ConsoleInput; //methode pour récupérer le fichier donnée en argument dans la console
	if((MessageErreur = PutString((char*)input)) == -1){ // verification si on a une erreur
		PutString("\n");
		PutString("Erreur lors du PutString, message d'erreur n° : ");
		PutString((char*)MessageErreur);
	}
	PutString("\n");
    return 0;
}

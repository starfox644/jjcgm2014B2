/*
 * Printf.c
 *  Fonction permettant a un utilisateur d'afficher un message sur la console en insérant une variable dans le texte.
 * %d => integer
 * %c => char
 * %s => char*
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */
#include "Printf.h"
#include "syscall.h"

void Printf(char* messageVoulu, ...){

	int i = 0, j = 0, k;
	int variable;
	char *value, buffer[MAX_LENGH];

	arg_start();

	// tant que l'on est pas a la fin du message
	while (messageVoulu[i] != '\0') {
		// si on a un %, il faut recuperer la variable
		// la transformer en chaine et la rajouter au resultat
		if (messageVoulu[i] == '%') {
			i++;
			variable = arg_arg();

			switch(messageVoulu[i]) {
			case 'd': // un entier
				value = Itoa(variable);
				break;

			case 'i':
				value = Itoa(variable);
				break;

			case 'c': // un char
				value[0] = (char) variable;
				value[1] = '\0';
				break;

			case 's': // un string
				value = (char*)variable;
				break;
			}

			k = 0;
			while (value[k] != '\0') {
				buffer[j] = value[k];
				j++;
				k++;
			}
		}
		else {
			buffer[j] = messageVoulu[i];
			j++;
		}

		i++;
	}
	PutString(buffer);

	// on vide le buffer (sinon, il y a des affichages en trop
	for (i = 0; i < MAX_LENGH; i++) {
		buffer[i] = '\0';
	}

}


void* Scanf(char* typeVariable, void *variable){
	if(typeVariable[0] == '%'){
		switch(typeVariable[1]){
			case 'd' :	//cas de récupération d'un int
				GetString((char*)variable,sizeof(int));
				variable = (void*)Atoi((char*)variable);
				return variable;
				break;
			case 'c' : //cas de récupération d'un char
				GetString(variable,1 * sizeof(char));
				StrCpy(variable,(char*)variable);
				return variable;
				break;
			case 's' : //cas de récupération d'un string
				GetString((char*)variable,MAX_LENGH);
				return variable;
				break;
			default :
				Printf("\nType de variable non reconnu\n",0);
				return 0;
				break;
		}
	}else{
		return 0;
	}
	return 0;
}

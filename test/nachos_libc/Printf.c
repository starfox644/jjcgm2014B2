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

int Printf (char* messageVoulu, ...){

	int i = 0, j = 0, k;
	int variable, nbArg;
	char *value, buffer[MAX_LENGH];

	arg_start();
	nbArg = 0;

	// tant que l'on est pas a la fin du message
	while (messageVoulu[i] != '\0') {
		// si on a un %, il faut recuperer la variable
		// la transformer en chaine et la rajouter au resultat
		if (messageVoulu[i] == '%') {
			nbArg++;

			if (nbArg < 4) {
				i++;
				variable = arg_arg ();

				switch (messageVoulu[i]) {
				case 'd': // un entier
					value = Itoa (variable);
					break;

				case 'i':
					value = Itoa (variable);
					break;

				case 'c': // un char
					value[0] = (char) variable;
					value[1] = '\0';
					break;

				case 's': // un string
					value = (char*) variable;
					break;
				}

				k = 0;
				while (value[k] != '\0') {
					buffer[j] = value[k];
					j++;
					k++;
				}
			}
			else // si on veut lire un 4eme argument
				return -1;
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

	return 0;
}

int Scanf (char* typeVariable, ...) {
	void* variable;
	char tmp[MAX_LENGH];
	int i, nbArg;

	i = 0;
	arg_start ();
	nbArg = 0;

	// tant qu'on est pas a la fin de la liste de variables a lire
	while (typeVariable[i] != '\0') {
		//si on lit un %, on va regarder la lettre apres pour savoir
		//le type de variable a lire
		if (typeVariable[i] == '%') {
			nbArg++;

			if (nbArg < 4) {
				variable = (void*) arg_arg ();
				i++;

				switch (typeVariable[i]) {
				case 'i':
				case 'd':	//cas de récupération d'un int
					GetString(tmp, MAX_LENGH);
					*((int*) variable) = Atoi (tmp);
					break;
				case 'c': //cas de récupération d'un char
					GetString(tmp, MAX_LENGH);
					((char*) variable)[0] = tmp[0];
					((char*) variable)[1] = '\0';
					break;
				case 's': //cas de récupération d'un string
					GetString ((char*) variable, MAX_LENGH);
					break;
				default:
					return -1;
					break;
				}
			}
			else // si on a plus de 3 arguments a lire, on a une erreur
				return -1;
		}

		i++;
	}

	return 0;
}

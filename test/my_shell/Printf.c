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

void Printf(char* messageVoulu, void* variable){

	int i = 0;
	char buffer[MAX_LENGH],bufferSuite[MAX_LENGH];
	//tant que le mot n'est pas finis ou que l'on ne trouve pas un % on continue
	while(messageVoulu[i] != '%' || messageVoulu[i] != '\0'){
		i++;
	}
	if(messageVoulu[i] == '\0'){//le mot est fini donc on affiche le message tout seul
		PutString(messageVoulu);
	}
	if(messageVoulu[i] == '%'){
		switch (messageVoulu[i+1]){
			case 'd'://on a un entier a afficher

				StrNCpy(messageVoulu, buffer, i - 1);
				PutString(buffer);
				PutInt((int) variable);
				i = i+2;//on ce décale pour etre apres le %d
				if(messageVoulu[i] == '\0'){// c'est la fin de notre message a afficher donc on stop
					return;
				}else{ //message non finis on continue d'afficher le reste du texte
					int j = 0;
					while(messageVoulu[i] != '\0'){
						bufferSuite[j] = messageVoulu[i];
						j++;
						i++;
					}
					PutString(bufferSuite);
					return;
				}
				break;
			case 'c':// on a un char a afficher
				break;
			case 's':// on a un string a afficher
				break;
		}
	}

}

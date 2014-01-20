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

void Printf(char* messageVoulu, void* variable){

	int i = 0;
	char buffer[MAX_LENGH],bufferSuite[MAX_LENGH];
	//tant que le mot n'est pas finis ou que l'on ne trouve pas un % on continue
	while(messageVoulu[i] != '%' && messageVoulu[i] != '\0'){
		buffer[i] = messageVoulu[i];
		i++;
	}
	if(variable == 0 || messageVoulu[i] == '\0'){//le mot est fini donc on affiche le message tout seul
		PutString(messageVoulu);
	}else{
		if(messageVoulu[i] == '%'){
			switch (messageVoulu[i+1]){
				case 'd'://on a un entier a afficher
					StrNCpy(messageVoulu, buffer, i - 1);
					PutString(buffer);
					variable = Itoa((int)variable);//on transforme le int en string pour pouvoir l'afficher.
					PutString((char*) variable);
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

				case 'i'://on a un entier a afficher
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
					StrNCpy(messageVoulu, buffer, i - 1);
					PutString(buffer);
					PutString((char*)variable);
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

				case 's':// on a un string a afficher
					StrNCpy(messageVoulu, buffer, i - 1);
					PutString(buffer);
					PutString((char*) variable);
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

			}
		}
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

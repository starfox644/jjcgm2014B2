/*
 * Programme utilisateur de shell pour que les utilisateurs ai un interface.
 */
#include "eval.h"
int main (){


	char buffer[MAX_CHAR];
	//char *tabBuffer[MAX_CHAR];
	int i,j = 0,k = 0;
	init_jobs();	//on initialise le tableau des jobs
    while (1){
    	if(j == MAX_CHAR){
    		j = 0;
    	}
    	i = 0;
    	PutString("<Nachos_Shell>");	//affichage du prompt
    	k = j;
    	do{// tant que l'on appuie pas sur entrée on lis les caractères entrés dans la console.
    		buffer[i] = (char) GetChar();	//on ajoute les caractères lu au buffer
    	}while (buffer[i++] != '\n');
    	buffer[--i] = '\0';

    	//on a finis de lire la commande entrée a la console
    	evalLine((char*)buffer);
    }
    return 0;
}

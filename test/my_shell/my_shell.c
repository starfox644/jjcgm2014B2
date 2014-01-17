/*
 * Programme utilisateur de shell pour que les utilisateurs ai un interface.
 */
#include "my_shell.h"
int main (){


	char buffer[MAX_CHAR];
	int i;
    while (1){
    	i = 0;
    	PutString("<Nachos_Shell>");	//affichage du prompt
    	do{// tant que l'on appuie pas sur entrée on lis les caractères entrés dans la console.
    		buffer[i] = (char) GetChar();	//on ajoute les caractères lu au buffer
    	}while (buffer[i++] != '\n');
    	buffer[--i] = '\0';
    	//on a finis de lire la commande entrée a la console
    	evalLine((char*)buffer);
    }
    return 0;
}

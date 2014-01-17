/*
 * Programme utilisateur de shell pour que les utilisateurs ai un interface.
 */
#include "my_shell.h"
int main (){


	char buffer[MAX_CHAR];
	char *tabBuffer[MAX_CHAR];
	int i,j = 0,k = 0;
    while (1){
    	if(j == MAX_CHAR){
    		j = 0;
    	}
    	i = 0;
    	PutString("<Nachos_Shell>");	//affichage du prompt
    	k = j;
    	do{// tant que l'on appuie pas sur entrée on lis les caractères entrés dans la console.
    		buffer[i] = (char) GetChar();	//on ajoute les caractères lu au buffer
    		if(ChrCmp(buffer[i],'\273') == 1){	// a revoir pour la gestion d'un historique des commandes rentrées
    			if(k > 0 && k < MAX_CHAR -1){
    				Printf("%s \n",tabBuffer[k-1]);
    			}else{
    				k = 0;
    				Printf("%s \n",tabBuffer[k]);
    			}
    		}
    	}while (buffer[i++] != '\n');
    	buffer[--i] = '\0';
    	tabBuffer[j] = buffer;
    	j++;

    	//on a finis de lire la commande entrée a la console
    	evalLine((char*)buffer);
    }
    return 0;
}

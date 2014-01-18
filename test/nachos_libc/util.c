/*
 * util.c
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "util.h"

int Atoi(char* chaine){
	int i = 0;
	int entier = 0,negatif = 0;
	PutString(chaine);
	while(chaine[i] != '\0'){
	    if(chaine[i] >= '0' && chaine[i] <= '9'){
	    	entier = entier * 10 + chaine[i] -'0';
	    }else if(chaine[0] == '-' && i==0){
	        negatif = 1;
	    }else{
	        break;
	    }
	    i++;
	}
	if(negatif == 1){
		entier = entier * -1;
	}
	PutInt(entier);
	PutString("Fin atoi\n");
	return entier;
}

 char* reverse(char s[])
 {
     int i, j;
     char c;
     for (i = 0, j = StrLengh(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
     return s;
 }

char* Itoa(int variable){
	 	 int i,sign;
	 	 char *s = malloc(MAX_LENGH * sizeof(char));
	 	 if(s == 0){
	 		 PutString("Erreur Malloc\n");
	 		 Exit(0);
	 	 }
	     if ((sign = variable) < 0)
	         variable = -variable;  // on met notre variable positive pour faire notre transformation
	     i = 0;
	     do {  //on crée les variables mais dans l'autre sens
	         s[i++] = variable % 10 + '0';
	     } while ((variable /= 10) > 0);
	     if (sign < 0)
	         s[i++] = '-';
	     s[i] = '\0';
	     s = reverse(s);	//on le remet dans le bon sens
	     return s;
}



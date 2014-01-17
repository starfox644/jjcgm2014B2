/*
 * String.cc
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#include "String.h"

int StrCmp(char* element1, char* element2){
	int i = 0,j = 0;
	while(element1[i] != '\0' || element2[j] != '\0'){
		if(element1[i] != element2[j]){
			return 0;
		}
		i++;
		j++;
	}
	if(i == j){// les éléments sont égaux
		return 1;
	}else{
		return 0;//les éléments sont différents
	}
}

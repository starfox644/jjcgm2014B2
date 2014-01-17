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

int ChrCmp(char element1, char element2){
	if(element1 != element2){
			return 0;
	}else{// les éléments sont égaux
		return 1;
	}
}

int StrCpy(char *source, char *destination){
	int i = 0;
	while(source[i] != '\0'){
		destination[i] = source[i];
		i++;
	}
	return i;
}

int StrNCpy(char *source,char *destination,int taille){
	int i = 0;
	while(i < taille){
		destination[i] = source[i];
		i++;
	}
	destination[i+1] = '\0'; // pour faire une fin de mot au cas ou
	return i;
}

int StrNDCpy(char *source,char *destination,int debut, int taille){
	int i = debut;
	if(debut < taille){
		while(i < taille){
			destination[i] = source[i];
			i++;
		}
		destination[i+1] = '\0'; // pour faire une fin de mot au cas ou
		return i;
	}else
		return -1;
}

int StrLengh(char * element){
	int compteur = 0;
	while(element[compteur] != '\0'){
		compteur++;
	}
	return compteur;
}

char* StrChr(char *element, char elementATrouve){
	int compteur = 0;
	while(element[compteur] != '\0' && element[compteur] != elementATrouve){
		compteur++;
	}
	if(element[compteur] == elementATrouve){
		return &element[compteur];
	}else{
		return 0;
	}
}

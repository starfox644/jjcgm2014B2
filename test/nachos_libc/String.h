/*
 * String.h
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef STRING_H_
#define STRING_H_

/*
 * Le but de cette fonction et de pouvoir comparer si 2 string sont bien égaux.
 */
int StrCmp(char*, char*);

/*
 * Le but de cette fonction et de pouvoir comparer si 2 char sont bien égaux
 */
int ChrCmp(char element1, char element2);
/*
 * Le but de cette fonction et de pouvoir copier 2 char
 */
int StrCpy(char*, char*);

/*
 * Le but de cette fonction et de pouvoir copier 2 char* sur une taille défini
 */
int StrNCpy(char*,char*,int);
/*
 * Le but de cette fonction et de pouvoir copier 2 char* depuis un point de debut définir et d'une taille voulu
 * Exemple : si on veut juste de la deuxieme lettre a la 4 eme c'est faisable.
 */
int StrNDCpy(char *source,char *destination,int debut, int taille);

/*
 *Le but de cette fonction et de calculer le nombre de caractère d'un char* et de le retourner.
 */
int StrLengh(char * element);

/*
 * Le but de cette fonction et de trouver l'emplacement d'un caractère dans un string et de renvoyer sa position ou 0 si le caractère n'est pa présent
 */
char* StrChr(char *element, char elementATrouve);

#endif /* STRING_H_ */

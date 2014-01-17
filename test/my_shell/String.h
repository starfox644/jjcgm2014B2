/*
 * String.h
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef STRING_H_
#define STRING_H_

/*
 * Le but de cette fonction et de pouvoir comparer si 2 string son bien égaux.
 */
int StrCmp(char*, char*);
/*
 * Le but de cette fonction et de pouvoir copier 2 char
 */
int StrCpy(char*, char*);

/*
 * Le but de cette fonction et de pouvoir copier 2 char* sur une taille défini
 */
int StrNCpy(char*,char*,int);



#endif /* STRING_H_ */

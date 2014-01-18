/*
 * userTestLib.c
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "Printf.h"
#include "syscall.h"

int main(){

	int d;
	char *c;
	char *s;
	Printf("Veuillez entrer un entier svp ?\n", 0);
	Scanf("%d", &d);
	Printf("Entier entré : %d\n",(void*)d);
	Printf("Veuillez entrer un char svp ?\n", 0);
	Scanf("%c", &c);
	Printf("Char entré : %c\n",c);
	Printf("Veuillez entrer un string svp ?\n", 0);
	Scanf("%s", &s);
	Printf("String entré : %s\n",s);

	return 0;
}




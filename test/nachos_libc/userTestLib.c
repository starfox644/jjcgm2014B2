/*
 * userTestLib.c
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "Printf.h"
#include "syscall.h"

int main(){

	int d = 12;
	char c = 'c';
	char *s = "toto";
	int at;
	char *it;

	at = Atoi("123");
	it = (char*) malloc(32 * sizeof(char));
	it = Itoa(d);

	Printf("entier = %d / caractere = %c / chaine = %s\n", d, c, s);
	Printf("at = %d\n",(void*)at);
	Printf("it = %s\n",it);

	d = 0;
	c = 0;
	s = (char*)malloc(MAX_LENGH * sizeof(char));

	Printf("Veuillez entrer un entier svp ?\n", 0);
	Scanf("%d", &d);
	Printf("Entier entré : %d\n",(void*)d);
	Printf("Veuillez entrer un char svp ?\n", 0);
	Scanf("%c",&c);
	Printf("Char entré : %c\n",&c);
	Printf("Veuillez entrer un string svp ?\n", 0);
	Scanf("%s",s);
	Printf("String entré : %s\n",s);
	//liberation des malloc
	free(s);
	free(it);

	return 0;
}



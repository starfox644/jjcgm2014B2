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
	Printf("d = %d\n",(void*)d);
	Printf("c = %c\n",&c);
	Printf("s = %s\n",s);

	PutString("Atoi\n");
	int at;
	at = Atoi("123");
	Printf("at = %d\n",(void*)at);
	PutString("Malloc\n");
	char *it = (char*) malloc(32 * sizeof(char));
	PutString("Itoa\n");
	it = Itoa(d);
	Printf("it = %s\n",it);



	d = 0;
	c = 0;
	s = malloc(MAX_LENGH * sizeof(int));

	/*Printf("Veuillez entrer un entier svp ?\n", 0);
	Scanf("%d", (void*)d);
	Printf("Entier entré : %d\n",(void*)d);
	*/Printf("Veuillez entrer un char svp ?\n", 0);
	Scanf("%c",&c);
	Printf("Char entré : %c\n",&c);
	Printf("Veuillez entrer un string svp ?\n", 0);
	Scanf("%s",s);
	Printf("String entré : %s\n",s);
	return 0;
}




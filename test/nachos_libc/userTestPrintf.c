/*
 * userTestLib.c
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "Printf.h"
#include "syscall.h"

int main() {

	int d = 12;
	char c = 'c';
	char *s = "toto";
	int at;
	char *it;
	int resultat;

	at = Atoi("123");
	it = (char*) malloc(32 * sizeof(char));

	if (it == 0) {
		Printf("Erreur alloc\n");
		return -1;
	}

	it = Itoa(d);

	resultat = Printf("Et %d et %d\n", 1, 2);
	Printf("affichage : %s\n\n", resultat ? "erreur" : "ok");

	resultat = Printf("entier = %d / caractere = %c / chaine = %s\n", d, c, s);
	Printf("affichage : %s\n\n", resultat ? "erreur" : "ok");

	resultat = Printf("at = %d\n",(void*)at);
	Printf("affichage : %s\n\n", resultat ? "erreur" : "ok");

	resultat = Printf("it = %s\n",it);
	Printf("affichage : %s\n\n", resultat ? "erreur" : "ok");

	resultat = Printf("Et %d et %d et %d et %d\n", 1, 2, 3, 4);
	Printf("affichage : %s\n\n", resultat ? "erreur" : "ok");

	free(it);

	return 0;
}

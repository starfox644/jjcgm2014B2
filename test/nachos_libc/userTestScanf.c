/*
 * userTestLib.c
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */
#include "Printf.h"
#include "syscall.h"

int main() {
	int a, b, resultat;
	char c;
	char *s, *t;

	a = 0;
	b = 0;
	s = (char*) malloc(MAX_LENGH * sizeof(char));
	t = (char*) malloc(MAX_LENGH * sizeof(char));

	if (s == 0 || t == 0) {
		Printf("Erreur alloc\n");
		return -1;
	}

	Printf("Veuillez entrer un entier svp ?\n");
	resultat = Scanf("%d", &a);
	if (resultat)
		Printf("Erreur lecture\n\n");
	else
		Printf("Entier entré : %d\n\n", a);


	Printf("Veuillez entrer un char svp ?\n");
	resultat = Scanf("%c", &c);
	if (resultat)
		Printf("Erreur lecture\n\n");
	else
		Printf("Char entré : %c\n\n", c);


	Printf("Veuillez entrer un string svp ?\n");
	resultat = Scanf("%s", s);
	if (resultat)
		Printf("Erreur lecture\n\n");
	else
		Printf("String entré : %s\n\n", s);


	Printf("Veuillez entrer un caractere puis deux entier svp ?\n");
	resultat = Scanf("%c%d %i", &c, &b, &a);
	if (resultat)
		Printf("Erreur lecture\n\n");
	else
		Printf("lu : %c %d %i\n\n", c, b, a);


	Printf("Veuillez entrer deux chaines svp ?\n");
	resultat = Scanf("%s%s", s, t);
	if (resultat)
		Printf("Erreur lecture\n\n");
	else
		Printf("lu : %s / %s \n\n", s, t);


	Printf("Test erreur :\n");
	resultat = Scanf("%w", s);
	if (resultat)
		Printf("Erreur lecture\n\n");
	else
		Printf("String entré : %s\n\n", s);

	//liberation des malloc
	free(s);
	free(t);
	return 0;
}

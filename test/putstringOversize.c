#include "syscall.h"
#define MAX MAX_STRING_SIZE + 50

// Test quand une chaine de caracteres depasse la taille maximum autorisee (MAX_STRING_SIZE)
int main()
{
	int i;
	int cpt;
	char s[MAX];
	// On ajoute l'alphabet jusqu'a la taille entree dans max-1 (pour le \0)
	for (i=0; i < MAX-1; i++)
	{
		if (i%10 == 0)
			s[i] = '|';
		else
			s[i] = '-';
	}
	s[i+1] = '\0';
	cpt = PutString(s);
    PutString("\n");
    PutString("Nombre d'octets ecrits : ");
	PutInt(cpt);
	PutString("\n");
	// Affiche 1024 caracteres puis s'arrete.
    return 0;
}

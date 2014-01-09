#include "syscall.h"
#define MAX MAX_STRING_SIZE + 10
int main()
{
	// Test quand une chaine de caracteres depasse la taille maximum autorisee (MAX_STRING_SIZE)
	int i;
//	int cpt;
	char s[MAX];
	// On ajoute l'alphabet jusqu'a la taille entree dans max-1 (pour le \0)
	for (i=0; i < MAX-1; i++)
	{
		if (i%10 == 0)
			s[i] = '|';
		else if (i%10 == 1)
			s[i] = '1';
		else
			s[i] = '-';
	}
	s[i+1] = '\0';
	/*cpt = */PutString(s);
//	PutString("\n");
//	PutString("Nombre d'octets ecrits : ");
//	PutInt(cpt);
//	PutString("\n");
	// Affiche 1024 caracteres puis s'arrete.
    return 0;
}

#include "syscall.h"

// recupere une chaine de taille 5 ('\0' inclus) et l'affiche
int main()
{
	char s[5];

	if(GetString(s, 5) != -1)
		PutString(s);
	else
		PutString("Erreur de GetString");

	return 0;
}

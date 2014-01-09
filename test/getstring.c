#include "syscall.h"

int main()
{
	char s[5];

	// on recupere une chaine de taille 5 ('\0' inclus) et on l'affiche
	if(GetString(s, 5) != -1)
		PutString(s);
	else
		PutString("Erreur de GetString");

	return 0;
}

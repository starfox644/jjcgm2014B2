/*
 * Permet de tester des entrees de caracteres speciales,
 * avec les fichiers putstringHexa0xFF et putstringHexa0x01
 */
#include "syscall.h"

int main()
{
	char s[50];
	int nbLus;
	if((nbLus = GetString(s, 50)) == -1){ // verification si on a une erreur
		PutString("\n");
		PutString("Erreur lors du PutString");
	}
	else
	{
		PutString("nombre de caracteres lus : \n");
		PutInt(nbLus);
		PutString("\n");
		PutString(s);
	}
	PutString("\n");
    return 0;
}

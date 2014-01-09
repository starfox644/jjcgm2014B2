#include "syscall.h"

int main()
{
	// affichage de la chaine de caractere a l'adresse 9500
	char * ptr;
	ptr = (char*)9500;//taille a modifier si on change la taille de la mémoire virtuelle

	if(PutString(ptr) == -1)
		PutString("Erreur de PutString");

    return 0;
}

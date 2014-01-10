#include "syscall.h"

// tente d'afficher une chaine a une adresse invalide
int main()
{
	char * ptr;
	ptr = (char*)9500;//taille a modifier si on change la taille de la mémoire virtuelle

	if(PutString(ptr) == -1)
		PutString("Erreur systeme de PutString\n");
	else
		PutString(ptr);

    return 0;
}

#include "syscall.h"

int main()
{
	// affichage de la chaine de caractere a l'adresse 9500
	char * ptr;
	ptr = (char*)9500;//taille a modifier si on change la taille de la mémoire virtuelle
	PutString(ptr);
    return 0;
}

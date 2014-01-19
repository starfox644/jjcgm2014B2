#include "syscall.h"

// provoque une erreur dans un appel systeme en debordant de la memoire
int main()
{
	char * ptr;
	ptr = (char*)1500000;//taille a modifier si on change la taille de la mémoire virtuelle

	if(GetString(ptr,10) == -1)
		PutString("Erreur systeme de GetString\n");
	else
		PutString(ptr);

	return 0;
}

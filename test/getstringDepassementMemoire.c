#include "syscall.h"

int main()
{
	char * ptr;
	ptr = (char*)9500;//taille a modifier si on change la taille de la mémoire virtuelle
	GetString(ptr,10);
    return 0;
}

#include "syscall.h"

// Teste la creation de thread avec une mauvaise adresse de fonction
int main()
{
	int a = 0;
	if(UserThreadCreate((void(*)(void*))500000, &a) == -1)
	{
		PutString("Erreur de creation de thread.\n");
	}
	else
	{
		PutString("Creation reussie.\n");
	}
    return 0;
}

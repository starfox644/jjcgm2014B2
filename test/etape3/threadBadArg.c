#include "syscall.h"

// Teste la creation de thread avec une mauvaise adresse d'argument


void f(void *a)
{
	UserThreadExit();
}

int main()
{
	if(UserThreadCreate(f, (void*)-1) == -1)
	{
		PutString("Erreur de creation de thread.\n");
	}
	else
	{
		PutString("Creation reussie.\n");
	}
    return 0;
}

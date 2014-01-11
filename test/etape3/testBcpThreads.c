#include "syscall.h"

/**
 * 	Teste le lancement de deux threads executant la meme fonction.
 * 	Le premier affiche des 5 et le second affiche des 6.
 */

void f(void* arg)
{
	int* n = (int*)arg;
	PutInt(*n);
	PutString("\n");
	UserThreadExit();
}

int main()
{
	int i;
	for(i = 0 ; i < 100 ; i++)
	{
		if(UserThreadCreate(f, &i) == -1)
		{
			PutString("Echec de creation du thread :\n");
			PutInt(i);
			PutString("\n");
		}
	}
    return 0;
}

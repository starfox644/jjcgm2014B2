#include "syscall.h"

/**
 * 	Teste le lancement de deux threads executant la meme fonction.
 * 	Le premier affiche des 5 et le second affiche des 6.
 */

void f(void* arg)
{
	int* n = (int*)arg;
	int i;
	for(i = 0 ; i < 50 ; i++)
	{
		PutInt(*n);
	}
	PutString("\n");
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadMulti : \n");
	PutString("Lance 2 threads executant la meme fonction.\n");
	PutString("Doit afficher des 5 et des 6\".\n");
	PutString("-----------------------------------------\n");
	int a = 5;
	int b = 6;
	UserThreadCreate(f, &a);
	UserThreadCreate(f, &b);
    return 0;
}

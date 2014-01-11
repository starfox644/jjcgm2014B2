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
	UserThreadExit();
}

int main()
{
	int a = 5;
	int b = 6;
	UserThreadCreate(f, &a);
	UserThreadCreate(f, &b);
    return 0;
}

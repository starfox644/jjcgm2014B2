#include "syscall.h"
#define NB_THREADS 100

/**
 * 	Teste le lancement de deux threads executant la meme fonction.
 * 	Le premier affiche des 5 et le second affiche des 6.
 */
void f(void* arg)
{
	int n = *((int*)arg);
	PutInt(n);
	PutString("\n");
	UserThreadExit(0);
}

int main()
{
	int i;
	int nb[NB_THREADS];
	for(i = 0 ; i < NB_THREADS ; i++)
	{
		nb[i] = i;
		if(UserThreadCreate(f, &nb[i]) == -1)
		{
			PutString("Echec de creation du thread :\n");
			PutInt(i);
			PutString("\n");
		}
	}
	PutString("thread main se termine\n");
    return 0;
}

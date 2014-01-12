#include "syscall.h"
#define NB_THREADS 100

/**
 * 	Teste le lancement de nombreux threads, chacun affichant un numero different.
 * 	Si certains threads ne peuvent pas etre lances, leur numero est affiche.
 */

sem_t s;

void f(void* arg)
{
	int n = *((int*)arg);
	SemWait(&s);
	PutInt(n);
	PutString("\n");
	SemPost(&s);
	UserThreadExit(0);
}

int main()
{
	int i;
	int nb[NB_THREADS];
	SemInit(&s, 1);
	for(i = 0 ; i < NB_THREADS ; i++)
	{
		nb[i] = i;
		if(UserThreadCreate(f, &nb[i]) == -1)
		{
			SemWait(&s);
			PutString("Echec de creation du thread : ");
			PutInt(i);
			PutString("\n");
			SemPost(&s);
		}
	}
	PutString("thread main se termine\n");
    return 0;
}

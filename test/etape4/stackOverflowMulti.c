#include "syscall.h"

/*
 * Dans le main, creation d'un processus executant le programme userThreadMulti
 * puis d'un thread qui execute f, qui entraine l'erreur "StackOverFlow".
 * On remarque que le processus (qui execute userThreadMulti), continu a s'executer
 * alors que l'autre se termine correctement.
 */
void f(void *arg)
{
	int* i = (int*)arg;
	int j = *i;
	if (*i < 200)
	{
		j = (*i)+1;
		f(&j);
	}
}

int main()
{
	int i = 0;
	if(ForkExec("etape4/userThreadMulti") == -1)
	{
		PutString("erreur fork exec\n");
	}
	int tidf = UserThreadCreate(f, &i);
	if(UserThreadJoin(tidf, 0) == -1)
		PutString("Erreur join\n");
	PutString("fin main!\n");
    return 0;
}

#include "syscall.h"


void f(void *arg)
{
	//PutString("Debut f\n");
	int* i = (int*)arg;
	int j = *i;
	if (*i < 200)
	{
		j = (*i)+1;
		f(&j);
	}
}

void g(void *arg)
{
	int i;
	//PutString("debut g\n");
	for ( i  = 0 ; i < 1000 ; i++);
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

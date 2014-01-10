#include "syscall.h"

void f(void* arg)
{
	int* n = (int*)arg;
	if(PutString("lalaland\n") == -1)
		PutString("Erreur putstring\n");
	if(PutString("argument : \n") == -1)
		PutString("Erreur putstring\n");
	PutInt(*n);
	PutChar('\n');
	*n = 1;
	UserThreadExit();
}

int main()
{
	int a = 0;
	UserThreadCreate(f, &a);
	while(a != 1);
    return 0;
}

#include "syscall.h"

void f(void* arg)
{
	if(PutString("lalaland\n") == -1)
		PutString("Erreur putstring\n");
	if(PutString("argument : \n") == -1)
		PutString("Erreur putstring\n");
	PutInt(*((int*)arg));
}

int main()
{
	int a = 5;
	UserThreadCreate(f, &a);
	while(1);
    return 0;
}

#include "syscall.h"

/**
 *	Lance 3 threads en parallele affichant leur tid. */


void fin(void* arg)
{
	PutString("fin, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadExit();
}

void g(void* arg)
{
	PutString("g, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadExit();
}

void f(void* arg)
{
	PutString("f, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadExit();
}

int main()
{
	PutString("Main, Tid : \n");
	PutInt(GetTid());
	PutString("\n");
	UserThreadCreate(f, 0);
	UserThreadCreate(g, 0);
	UserThreadCreate(fin, 0);
    return 0;
}

#include "syscall.h"

void f(void* arg)
{
	int i;
	for (i = 0 ; i < 80 ; i++);
	PutChar('a');
	PutChar('b');
	PutChar('c');
	PutChar('d');

}

void g(void* arg)
{
	PutChar('1');
	PutChar('2');
	PutChar('3');
	PutChar('4');
}

int main()
{
	int tidf, tidg;
	PutString("\n");
	tidf = UserThreadCreate(f, 0);
	tidg = UserThreadCreate(g, 0);
	if(UserThreadJoin(tidf, 0) == -1)
			PutString("Erreur join du main sur f\n");
	if(UserThreadJoin(tidg, 0) == -1)
				PutString("Erreur join du main sur g\n");
	PutString("\n");
    return 0;
}

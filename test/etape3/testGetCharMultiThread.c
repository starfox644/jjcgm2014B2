#include "syscall.h"

typedef struct Arg {
	int  wait;
	char a;
} Arg;

void f(void* arg)
{
	PutChar('5');
	Arg *param = (Arg*) arg;
	PutChar(param->a);
	PutChar('6');
	param->wait = 0;
	PutChar('7');
	UserThreadExit();
}

int main()
{
	Arg arg;
	PutChar('1');
	arg.wait = 1;
	PutChar('2');
	arg.a = GetChar();
	PutChar('3');
	UserThreadCreate(f, &arg);
	PutChar('4');
	while(arg.wait);
	PutChar('8');
    return 0;
}

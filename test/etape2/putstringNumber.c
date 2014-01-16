#include "syscall.h"

int main()
{
	// test pour voir si les chiffres sont bien pris en compte et surtout le -1 et après
	PutString("123456789\n");
	PutString("10 11 12 -2 -3 -1 25 30\n");
    return 0;
}

#include "syscall.h"

int main()
{
	char s[5];


	GetString(s, 5);
	PutString(s);

	return 0;
}

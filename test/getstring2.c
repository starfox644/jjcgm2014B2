#include "syscall.h"

int main()
{
	char s[2];


	GetString(s, 5);
	PutString(s);

	return 0;
}

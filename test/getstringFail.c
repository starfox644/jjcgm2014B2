#include "syscall.h"

int main()
{
	char s[5];

	if(GetString(s, 2000000000) != -1)
		PutString(s);
	else
		PutString("Erreur de GetString");

	return 0;
}

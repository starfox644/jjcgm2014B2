#include "syscall.h"

int main()
{
	int i;
	char s[MAX_STRING_SIZE+1];
	for (i=0; i < MAX_STRING_SIZE+1; i++)
		s[i] = ('a'+i)%26;
	// Test with oversize string
	PutString(s);
    return 0;
}

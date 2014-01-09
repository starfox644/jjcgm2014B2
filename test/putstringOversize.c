#include "syscall.h"

int main()
{
	int i, max = MAX_STRING_SIZE+10;
	char s[max];
	for (i=0; i < max-1; i++)
		s[i] = 'a'+ (i%26);
	s[i+1] = '\0';
	// Test with oversize string
	PutString(s);
    return 0;
}

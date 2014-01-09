#include "syscall.h"

int main()
{
	int c;
	while((c = GetChar()) != -1)
		PutChar((char)c);
    return 0;
}

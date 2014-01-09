#include "syscall.h"

int main()
{
    char c;

    do {
    	c = (char) GetChar();
    	PutChar(c);
    } while (c != 'q');
    return 0;
}

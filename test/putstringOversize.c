#include "syscall.h"

int main()
{
	// Test with oversize string
	PutString("abcdefghijklmnopqrstuvwxyz\n");
    Halt();
}

#include "syscall.h"

int main()
{
	// Test with some weird string
	PutString("!@#([])=+}{~ŷÿ$£\n");
	PutString("%ê€§^¨%\n");
    Halt();
}

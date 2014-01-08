#include "syscall.h"

int main()
{
	// Test with some weird string
	PutString("!@#([])=+}{~ŷÿ$£");
	PutString("%ê€§^¨%");
    Halt();
}

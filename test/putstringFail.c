#include "syscall.h"

int main()
{
	// Test with a NULL pointer to ensure robustness
	PutString(0);
    Halt();
}

#ifdef CHANGED
#include "exit.h"
#include "system.h"

void do_exit(int returnCode)
{
	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");
	if(currentThread->space != NULL)
		delete currentThread->space;
	// stop the program
	interrupt->Halt ();
}

#endif

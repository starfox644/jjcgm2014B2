#ifdef CHANGED
#include "exit.h"
#include "system.h"

void do_exit(int returnCode)
{
	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");
	currentThread->space->s_nbThreads->P();
	currentThread->space->attente = true;
	currentThread->space->s_nbThreads->V();
	while(currentThread->space->getNbThreads() > 0)
	{
		currentThread->space->s_exit->P();
	}
	if(currentThread->space != NULL)
		delete currentThread->space;
	// stop the program
	interrupt->Halt ();
}

#endif

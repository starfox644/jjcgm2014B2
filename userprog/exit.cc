#ifdef CHANGED
#include "exit.h"
#include "system.h"
#include "process.h"

void do_exit(int returnCode)
{
	// indicates that the main thread is waiting for the others
	currentThread->space->s_nbThreads->P();
	currentThread->space->attente = true;
	currentThread->space->s_nbThreads->V();
	while(currentThread->space->getNbThreads() > 0)
	{
		// semaphore wait for waiting the others threads
		currentThread->space->s_exit->P();
	}

	if(currentThread->space != NULL)
		delete currentThread->space;

	s_process->P();

	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");

	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		removeProcess();
		threadToBeDestroyed = currentThread;
		s_process->V();
		currentThread->Yield();
	}
	else // the current thread is the last thread
	{
		removeProcess();
		s_process->V();
		// stop the program
		interrupt->Halt ();
	}
}

#endif

#ifdef CHANGED
#include "exit.h"
#include "system.h"

#ifdef step4
#include "process.h"
#endif

void do_exit(int returnCode)
{
#ifdef countNew
	printf("threads : created %d / destroyed %d / remaining %d\n", currentThread->nbThreadsCreated, currentThread->nbThreadsCreated - currentThread->nbNewThread, currentThread->nbNewThread);
	printf("addrspace : created %d / destroyed %d / remaining %d\n", currentThread->space->nbAddrspaceCreated, currentThread->space->nbAddrspaceCreated - currentThread->space->nbNewAddrspace, currentThread->space->nbNewAddrspace);
#endif
	// indicates that the main thread is waiting for the others
	currentThread->space->s_nbThreads->P();
	currentThread->space->attente = true;
	currentThread->space->s_nbThreads->V();
	while(currentThread->space->getNbThreads() > 0)
	{
		// semaphore wait for waiting the others threads
		currentThread->space->s_exit->P();
	}

//	currentThread->space->processRunning = false;

	if(currentThread->space != NULL)
	{
		delete currentThread->space;
		currentThread->space = NULL;
	}

//	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");

#ifdef step4
	s_process->P();
	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		removeProcess();
		// threadToBeDestroyed = currentThread;
		s_process->V();
		// currentThread->Yield();
		currentThread->Finish();
	}
	else // the current thread is the last thread
	{
		removeProcess();
		s_process->V();
		// stop the program
		interrupt->Halt ();
	}
#else
	interrupt->Halt ();
#endif
}

#endif

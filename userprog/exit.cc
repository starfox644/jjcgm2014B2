#ifdef CHANGED
#include "exit.h"
#include "system.h"

#ifdef CHANGED
#include "process.h"
#include "addrspace.h"
#include "threadManager.h"
#endif

/**
 * 	Execute l'appel systeme Exit.
 */
void do_exit(int returnCode)
{
	s_createProcess->P();
#ifdef countNew
	printf("threads : created %d / destroyed %d / remaining %d\n", Thread::getNbThreadsCreated(), Thread::getNbThreadsCreated() - Thread::getNbNewThread(), Thread::getNbNewThread());
	printf("addrspace : created %d / destroyed %d / remaining %d\n", AddrSpace::getNbAddrspaceCreated(), AddrSpace::getNbAddrspaceCreated() - AddrSpace::getNbNewAddrspace(), AddrSpace::getNbNewAddrspace());
#endif
	AddrSpace *space = currentProcess->getAddrSpace();
	currentProcess->threadManager->s_nbThreads->P();
	// notify that the main thread is waiting for another
	currentProcess->mainIsWaiting = true;
	currentProcess->threadManager->s_nbThreads->V();
	while(currentProcess->threadManager->getNbThreads() > 0)
	{
		// the thread waits for the others threads
		s_createProcess->V();
		space->s_exit->P();
		s_createProcess->P();
	}

//	currentThread->space->processRunning = false;

	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");
	currentProcess->freeAddrSpace();

#ifdef step4
	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		removeProcess();
		s_createProcess->V();
		currentThread->Finish();
	}
	else // the current thread is the last thread
	{
		removeProcess();
		s_createProcess->V();
		// stop the kernel
		interrupt->Halt ();
	}
#else
	interrupt->Halt ();
#endif
}

#endif

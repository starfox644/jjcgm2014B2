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
	//}

	printf("Program (Pid : %i) stopped with return code : %d\n", currentProcess->getPid(), returnCode);
	DEBUG('a',"Program exit");
	currentProcess->freeAddrSpace();

#ifdef step4
	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		removeProcess();
		processManager->removeAddrProcess(currentProcess);
		s_createProcess->V();
		currentProcess->semProc->V();
		currentThread->Finish();
	}
	else // the current thread is the last thread
	{
		removeProcess();
		processManager->removeAddrProcess(currentProcess);

		// stop the program
		s_createProcess->V();
		currentProcess->semProc->V();
		interrupt->Halt ();
	}
#else
	interrupt->Halt ();
#endif
<<<<<<< HEAD
	//}
=======

#ifdef countNew
	printf("threads : created %d / destroyed %d / remaining %d\n", Thread::getNbThreadsCreated(), Thread::getNbThreadsCreated() - Thread::getNbNewThread(), Thread::getNbNewThread());
	printf("addrspace : created %d / destroyed %d / remaining %d\n", AddrSpace::getNbAddrspaceCreated(), AddrSpace::getNbAddrspaceCreated() - AddrSpace::getNbNewAddrspace(), AddrSpace::getNbNewAddrspace());
#endif
>>>>>>> f0d1c315d1dc5dd8fb6b8e95df30695404d844ec
}

#endif

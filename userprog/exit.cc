#ifdef CHANGED
#include "exit.h"
#include "system.h"

#ifdef CHANGED
#include "process.h"
#include "addrspace.h"
#include "threadManager.h"
#include "synchconsole.h"
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

#ifdef CHANGED
	Printf("Program (Pid : %i) stopped with return code : %d\n", currentProcess->getPid(), returnCode);
#endif //CHANGED
	DEBUG('a',"Program exit");
	currentProcess->freeAddrSpace();

#ifdef step4
	// currentProcess isn't the last main process TODO better commentary ?
	if (processManager->getNbProcessRunning() > 1)
	{
		processManager->removeProcess(currentProcess);
		s_createProcess->V();
		currentProcess->semProc->V();
		currentThread->Finish();
	}
	else // the current process is the last process TODO better commentary ?
	{
		processManager->removeProcess(currentProcess);

		// stop the program
		s_createProcess->V();
		interrupt->Halt ();
	}
#else
	interrupt->Halt ();
#endif

#ifdef countNew
#ifdef CHANGED
	Printf("threads : created %d / destroyed %d / remaining %d\n", Thread::getNbThreadsCreated(), Thread::getNbThreadsCreated() - Thread::getNbNewThread(), Thread::getNbNewThread());
	Printf("addrspace : created %d / destroyed %d / remaining %d\n", AddrSpace::getNbAddrspaceCreated(), AddrSpace::getNbAddrspaceCreated() - AddrSpace::getNbNewAddrspace(), AddrSpace::getNbNewAddrspace());
#endif
#endif
}

#endif

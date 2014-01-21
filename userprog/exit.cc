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
#ifdef step4
	IntStatus oldLevel = interrupt->SetLevel (IntOff); /*****************************************/
#endif
	Printf("Tentative exit pid = %d name = %s\n", currentProcess->getPid(), currentThread->getName());
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
#ifdef CHANGED
	Printf("Program (Pid : %i) stopped with return code : %d\n", currentProcess->getPid(), returnCode);
#endif //CHANGED
	DEBUG('a',"Program exit");
	currentProcess->freeAddrSpace();

#ifdef step4
	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		Printf("[do_exit] Pas le dernier thread\n");
		removeProcess();
		processManager->removeAddrProcess(currentProcess);
		s_createProcess->V();
		currentProcess->semProc->V();
		(void) interrupt->SetLevel (oldLevel);/*****************************************/
		currentThread->Finish();
	}
	else // the current thread is the last thread
	{
		Printf("[do_exit] Dernier thread\n");
		removeProcess();
		processManager->removeAddrProcess(currentProcess);

		// stop the program
		s_createProcess->V();
		currentProcess->semProc->V();
		(void) interrupt->SetLevel (oldLevel);/*****************************************/
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

#ifdef CHANGED
#include "exit.h"
#include "system.h"

#ifdef CHANGED
#include "process.h"
#include "addrspace.h"
#include "threadManager.h"
#include "synchconsole.h"
#endif

void do_UserThreadExit(int);

/**
 * 	Execute l'appel systeme Exit.
 */
void do_exit(int returnCode)
{
	s_createProcess->P();
	Printf("********Tentative exit pid = %d name = %s\n", currentProcess->getPid(), currentThread->getName());
#ifdef step3
	// a thread is waiting, he will do exit
	if(!currentProcess->threadWaiting)
	{
#endif
		AddrSpace *space = currentProcess->getAddrSpace();
		currentProcess->threadManager->s_nbThreads->P();
		// notify that the main thread is waiting for another
		currentProcess->threadWaiting = true;
		// release the semaphore for threads which are waiting the end
		currentThread->s_join->V();
		currentProcess->threadManager->s_nbThreads->V();
		while(currentProcess->threadManager->getNbThreads() > 1)
		{
			// the thread waits for the others threads
			s_createProcess->V();
			space->s_exit->P();
			s_createProcess->P();
		}
		Printf("Program (Pid : %i) stopped with return code : %d\n", currentProcess->getPid(), returnCode);
		DEBUG('a',"Program exit");
		currentProcess->freeAddrSpace();
	#ifdef step4
		// currentThread isn't the last main thread
		if (processManager->getNbAddrProcess() > 1)
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
#ifdef step3
	}
	else
	{
		s_createProcess->V();
		// a thread is already waiting, just terminate the current thread
		do_UserThreadExit(returnCode);
	}
#endif

#ifdef countNew
#ifdef CHANGED
	Printf("threads : created %d / destroyed %d / remaining %d\n", Thread::getNbThreadsCreated(), Thread::getNbThreadsCreated() - Thread::getNbNewThread(), Thread::getNbNewThread());
	Printf("addrspace : created %d / destroyed %d / remaining %d\n", AddrSpace::getNbAddrspaceCreated(), AddrSpace::getNbAddrspaceCreated() - AddrSpace::getNbNewAddrspace(), AddrSpace::getNbNewAddrspace());
#endif
#endif
}

#endif

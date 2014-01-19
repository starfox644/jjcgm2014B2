#ifdef CHANGED
#include "exit.h"
#include "system.h"

#ifdef CHANGED
#include "process.h"
#include "addrspace.h"
#include "threadManager.h"
#endif

void do_exit(int returnCode)
{
	s_createProcess->P();
#ifdef countNew
	printf("threads : created %d / destroyed %d / remaining %d\n", Thread::getNbThreadsCreated(), Thread::getNbThreadsCreated() - Thread::getNbNewThread(), Thread::getNbNewThread());
	printf("addrspace : created %d / destroyed %d / remaining %d\n", AddrSpace::getNbAddrspaceCreated(), AddrSpace::getNbAddrspaceCreated() - AddrSpace::getNbNewAddrspace(), AddrSpace::getNbNewAddrspace());
#endif
	AddrSpace *space = currentProcess->getAddrSpace();
	//if (currentThread->isMainThread())
	//{
		// indicates that the main thread is waiting for the others
		currentProcess->threadManager->s_nbThreads->P();
		space->attente = true;
		currentProcess->threadManager->s_nbThreads->V();
		while(currentProcess->threadManager->getNbThreads() > 0)
		{
			// semaphore wait for waiting the others threads
			s_createProcess->V();
			space->s_exit->P();
			s_createProcess->P();
		}
	//}
	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");

#ifdef step4
	//s_createProcess->P();
	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		removeProcess();
		//threadToBeDestroyed = currentThread;
		//currentThread->space->processRunning = false;
		//s_createProcess->V();
		// currentThread->Yield();
		s_createProcess->V();
		currentThread->Finish();
	}
	else // the current thread is the last thread
	{
		removeProcess();
		//currentThread->space->processRunning = false;
		//s_createProcess->V();
		// stop the program
		s_createProcess->V();
		interrupt->Halt ();
	}
#else
	printf("Avant halt\n");
	interrupt->Halt ();
#endif
}

#endif

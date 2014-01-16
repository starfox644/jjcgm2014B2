#ifdef CHANGED
#include "exit.h"
#include "system.h"

#ifdef CHANGED
#include "process.h"
#include "addrspace.h"
#endif

void do_exit(int returnCode)
{
#ifdef countNew
	printf("threads : created %d / destroyed %d / remaining %d\n", Thread::getNbThreadsCreated(), Thread::getNbThreadsCreated() - Thread::getNbNewThread(), Thread::getNbNewThread());
	printf("addrspace : created %d / destroyed %d / remaining %d\n", AddrSpace::getNbAddrspaceCreated(), AddrSpace::getNbAddrspaceCreated() - AddrSpace::getNbNewAddrspace(), AddrSpace::getNbNewAddrspace());
#endif
	AddrSpace *space = currentThread->process->getAddrSpace();
	// indicates that the main thread is waiting for the others
	space->s_nbThreads->P();
	space->attente = true;
	space->s_nbThreads->V();
	while(space->getNbThreads() > 0)
	{
		// semaphore wait for waiting the others threads
		space->s_exit->P();
	}

//	currentThread->space->processRunning = false;

	if(currentThread->process->getAddrSpace() != NULL)
	{

	}

//	printf("Program stopped with return code : %d\n", returnCode);
	DEBUG('a',"Program exit");

#ifdef step4
	s_process->P();
	// currentThread isn't the last main thread
	if (getNbProcess() > 1)
	{
		removeProcess();
		//threadToBeDestroyed = currentThread;
		//currentThread->space->processRunning = false;
		s_process->V();
		// currentThread->Yield();
		currentThread->Finish();
	}
	else // the current thread is the last thread
	{
		removeProcess();
		//currentThread->space->processRunning = false;
		s_process->V();
		// stop the program
		interrupt->Halt ();
	}
#else
	interrupt->Halt ();
#endif
}

#endif

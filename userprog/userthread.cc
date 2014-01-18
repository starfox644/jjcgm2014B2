#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "machine.h"
#include "threadManager.h"

extern void do_exit(int returnCode);

/**
 * 		Implements system call UserThreadCreate.
 *    f : 		function address to execute in the MIPS processor
 *    arg : 	argument address for the f function, f will be called with arg
 */
int do_UserThreadCreate(int f, int arg)
{
	int n;
	bool error = false;
	int stackAddr;
	char buffer[10];
	// locks this function
	s_createProcess->P();
	AddrSpace* space = currentProcess->getAddrSpace();
	ASSERT(space != NULL);

	// allocate the new thread
	Thread *newThread = NULL;
	newThread = new Thread((char*)buffer);
	// allocation error
	if (newThread == NULL)
		error = true;

	// test the accessibility of the code and the argument
    error = error || !machine->ReadMem(f, sizeof(int), &n);
    if(!error)
    {
    	// accept arg 0 for passing NULL as argument
    	error = (arg != 0) && !machine->ReadMem(arg, sizeof(int), &n);
    	if(!error)
    	{
    		// get an address for the stack if possible
#ifdef step4
    		// for step 4 we allocate a part of virtual memory
    		stackAddr = space->allocThreadStack();
#else
    		// for step 3 we use a list of stack pointers
    		stackAddr = space->popAvailableStackPointer();
#endif
    		error = (stackAddr == -1);
    	}
    }

    if(!error)
    {
		// gets a tid for the thread
		newThread->tid = Thread::getTid();
		error = (newThread->tid == -1);
		if(!error)
		{
			// the new thread is in same process
			newThread->process = currentThread->process;
			currentProcess->threadManager->s_nbThreads->P();
			// add the thread to the process
			currentProcess->threadManager->addThread(newThread);
			currentProcess->threadManager->s_nbThreads->V();

			// sets initial argument of the thread
			newThread->setInitArg(arg);
			// sets user stack address of the thread, in the address space
			newThread->userStackAddr = stackAddr;
			// creation of the new thread, StartUserThread will be called with f
			newThread->Fork(StartUserThread, f);
			// end of critical section
			s_createProcess->V();
			return newThread->tid;
		}
    }
    // thread creation error
    delete newThread;
	// end of critical section
    s_createProcess->V();
    return -1;
}

/**
 * 		Launch an user thread, which have to execute the user function at the address f.
 */
static void StartUserThread(int f)
{
	// copy the arg in register 27 (reserved to OS) for saving it, will be load in r4 by startThread
	machine->WriteRegister(27, currentThread->getInitArg());
	// set PC to the function __startThread (in start.s)
	machine->WriteRegister(PCReg, THREAD_START_OFFSET);
	// set next PC
	machine->WriteRegister(NextPCReg, THREAD_START_OFFSET + 4);
	// set return address (none)
	machine->WriteRegister(31, -1);
	// set SP with the stack address of the thread
	machine->WriteRegister(StackReg, currentThread->userStackAddr);
	// set r26 (reserved to OS) to the function address for loading it later into pc
	machine->WriteRegister(26, f);
	machine->Run ();		// jump to the user progam at __startThread
}

/**
 * 		Implements system call UserThreadJoin.
 *		Wait the thread tid and store the return code at addrUser.
 *		If the thread isn't found in the process, -1 is returned.
 */
int do_UserThreadJoin(int tid, int addrUser)
{
	Thread* th;

	currentProcess->threadManager->s_userJoin->P();

	// search the waited thread in the process
	if ((th = currentProcess->threadManager->searchThread(tid)) == NULL)
	{
		// error if the thread wasn't found
		currentProcess->threadManager->s_userJoin->V();
		return -1;
	}
	else
	{
		// check if the thread is already waited by another
		if(!th->isFinished && th->wait)
		{
			// another thread is already waiting for this thread : error
			currentProcess->threadManager->s_userJoin->V();
			return -1;
		}
		else
		{
			// notify that we are waiting the thread
			th->wait = true;
			currentProcess->threadManager->s_userJoin->V();
			// wait while the thread doesn't finish
			th->s_join->P();
			// notify that we are no more waiting the thread
			th->wait = false;
			// set return code at address addrUser
			if (addrUser != 0)
				machine->WriteMem(addrUser, sizeof(int), th->getThreadReturn());
			th->s_join->V();
			return 0;
		}
	}
}

/**
 * 		Implements system call UserThreadExit.
 * 		status is given at a thread which wait for it with UserThreadJoin.
 */
void do_UserThreadExit(int status)
{
	if(!currentThread->isMainThread())
	{
		s_createProcess->P();
		AddrSpace* space = currentProcess->getAddrSpace();
		currentProcess->threadManager->s_nbThreads->P();
		// notify that the thread is finished in case of UserThreadJoin
		currentThread->isFinished = true;
		// remove the thread in the process
		currentProcess->threadManager->removeThread(currentThread);
		// if the main thread is waiting, notify the end of the thread
		if(currentProcess->mainIsWaiting)
			space->s_exit->V();
		// save the return status of the thread if another calls UserThreadJoin
		currentThread->setThreadReturn(status);
		currentProcess->threadManager->s_nbThreads->V();
	    s_createProcess->V();
		// terminates this thread
		currentThread->Finish();
	}
	else
	{
		// if the main thread calls userthreadexit, that stops the program
		do_exit(status);
	}
}

#endif

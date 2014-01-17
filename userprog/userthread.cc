#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "machine.h"
#include "threadManager.h"

extern void do_exit(int returnCode);

/**
*    f : 	function address to execute in the MIPS processor
*    arg : 	argument address for the f function, f will be called with arg
*/
int do_UserThreadCreate(int f, int arg)
{
	int n;
	bool error = false;
	int stackAddr;
	// locks this function
	s_create->P();
	AddrSpace* space = currentProcess->getAddrSpace();
	Thread *newThread = new Thread("test");
	// error allocation
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
    		stackAddr = space->allocThreadStack();
#else
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
			// the new thread shares the memory space with the current thread
			newThread->process = currentThread->process;
			currentProcess->threadManager->s_nbThreads->P();
			currentProcess->threadManager->addThread(newThread);
			currentProcess->threadManager->s_nbThreads->V();

			// sets initial argument of the thread
			newThread->setInitArg(arg);
			// sets user stack address of the thread, in the address space
			newThread->userStackAddr = stackAddr;
			// creation of the new thread, StartUserThread will be called with f
			newThread->Fork(StartUserThread, f);
#ifdef step4
			/*space->addrSpaceAllocator->printFreeList();
			space->addrSpaceAllocator->printBusyList();*/
#endif
			// end of critical section
			s_create->V();
			return newThread->tid;
		}
    }
    // thread creation error
    delete newThread;
	// end of critical section
    s_create->V();
    return -1;
}

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

void do_UserThreadExit(int status)
{
	if(!currentThread->isMainThread())
	{
		AddrSpace* space = currentProcess->getAddrSpace();
		// remove the thread in the address space
		currentProcess->threadManager->s_nbThreads->P();
		currentThread->isFinished = true;
		currentProcess->threadManager->removeThread(currentThread);
		// if the main thread is waiting, notify the end of the thread
		if(space->attente)
			space->s_exit->V();
		currentThread->setThreadReturn(status);
		currentProcess->threadManager->s_nbThreads->V();
		// terminates this thread
		currentThread->Finish();
	}
	else
	{
		// if the main thread calls userthreadexit, that stops the program
		do_exit(status);
	}
}


int do_UserThreadJoin(int tid, int addrUser)
{
	Thread* th;
	//AddrSpace *space = currentThread->process->getAddrSpace();

	currentProcess->threadManager->s_userJoin->P();
	// search the given thread in l_thread
	// tid does not exist : error
	if ((th = currentProcess->threadManager->searchThread(tid)) == NULL)
	{
		currentProcess->threadManager->s_userJoin->V();
		return -1;
	}
	else
	{
		//th = *it;
		// an other thread wait for this thread : error
		if(!th->isFinished && th->wait)
		{
			currentProcess->threadManager->s_userJoin->V();
			return -1;
		}
		else
		{
			th->wait = true;
			currentProcess->threadManager->s_userJoin->V();
			// wait while the thread doesn't finish
			th->s_join->P();

			th->wait = false;
			// set return code to this address addrUser
			if (addrUser != 0)
				machine->WriteMem(addrUser, sizeof(int), th->getThreadReturn());
			th->s_join->V();
			return 0;
		}
	}
}

#endif

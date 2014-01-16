#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "machine.h"

extern void do_exit(int returnCode);

/**
*    f : 		function address to execute in the MIPS processor
*    arg : 	argument address for the f function, f will be called with arg
*/
int do_UserThreadCreate(int f, int arg)
{
	int n;
	bool error = false;
	int stackAddr;
	// locks this function
	s_create->P();
	AddrSpace* space = currentThread->process->getAddrSpace();
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
    		stackAddr = space->popAvailableStackPointer();
    		error = (stackAddr == -1);
/*#ifdef step4
    		if(!error)
    		{
    			if(!space->map(stackAddr - UserStackSize, UserStackSize, true))
    				error = true;
    		}
#endif*/
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
			space->s_nbThreads->P();
			space->addThread(newThread);
			space->s_nbThreads->V();

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
	AddrSpace* space = currentThread->process->getAddrSpace();
	space->InitRegisters();
	space->RestoreState ();	// load page table register

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
		AddrSpace* space = currentThread->process->getAddrSpace();
		// remove the thread in the address space
		space->s_nbThreads->P();
		currentThread->isFinished = true;
		space->removeThread(currentThread);
		// if the main thread is waiting, notify the end of the thread
		if(space->attente)
			space->s_exit->V();
		space->s_nbThreads->V();
		currentThread->setThreadReturn(status);
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
	AddrSpace *space = currentThread->process->getAddrSpace();
	std::list<Thread*>::iterator it = space->l_threads.begin();
	space->s_userJoin->P();
	// search the given thread in l_thread
	while (it != space->l_threads.end() && (tid != (*it)->tid))
	{
		++it;
	}
	// tid does not exist : error
	if (it == space->l_threads.end())
	{
		space->s_userJoin->V();
		return -1;
	}
	else
	{
		th = *it;
		// an other thread wait for this thread : error
		if(!th->isFinished && th->wait)
		{
			space->s_userJoin->V();
			return -1;
		}
		else
		{
			th->wait = true;
			space->s_userJoin->V();
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

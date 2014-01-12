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
	s_create->P();
	Thread *newThread = new Thread("test");

	// test the accessibility of the code and the argument
    error = !machine->ReadMem(f, sizeof(int), &n);
    if(!error)
    {
    	// accept arg 0 for passing NULL as argument
    	error = (arg != 0) && !machine->ReadMem(arg, sizeof(int), &n);
    	if(!error)
    	{
    		// test the accessibility of the stack
    		error = !machine->ReadMem(2*currentThread->tid*f+STACK_OFFSET*PageSize, sizeof(int), &n);
    	}
    }

    if(!error)
    {
		// the new thread shares the memory space with the current thread
		newThread->space = currentThread->space;
		newThread->space->s_nbThreads->P();
		newThread->space->addThread(newThread);
		newThread->space->s_nbThreads->V();
		// sets initial argument of the thread
		newThread->setInitArg(arg);
		newThread->tid = Thread::nextTid;
		Thread::nextTid++;
		// creation of the thread, init and positionning in the file
		// the new thread executes StartUserThread (saving register)
		newThread->Fork(StartUserThread, f);
		s_create->V();
    	return newThread->tid;
    }
    else
    {
    	delete newThread;
    	s_create->V();
    	return -1;
    }
}

static void StartUserThread(int f)
{
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState ();	// load page table register

	// copy the arg in register 27 (reserved to OS) for saving it, will be load in r4 by startThread
	machine->WriteRegister(27, currentThread->getInitArg());
	// set PC
	machine->WriteRegister(PCReg, THREAD_START_OFFSET);
	// set next PC
	machine->WriteRegister(NextPCReg, THREAD_START_OFFSET + 4);
	// set return address (none)
	machine->WriteRegister(31, -1);
	// set SP
	machine->WriteRegister(StackReg, 2*currentThread->tid*f+STACK_OFFSET*PageSize);
	// set r26 (reserved to OS) to the function address
	machine->WriteRegister(26, f);
	machine->Run ();		// jump to the user progam
}

void do_UserThreadExit(int status)
{
	if(!currentThread->isMainThread())
	{
		// remove the thread in the address space
		currentThread->space->s_nbThreads->P();
		currentThread->space->removeThread(currentThread);
		// if the main thread is waiting, notify the end of the thread
		if(currentThread->space->attente)
			currentThread->space->s_exit->V();
		currentThread->space->s_nbThreads->V();
		// terminates this thread
		currentThread->Finish();
	}
	else
	{
		// if the main thread calls userthreadexit, that stops the program
		do_exit(status);
	}
}


int do_UserThreadJoin(int tid)
{
	Thread* th;
	std::list<Thread*>::iterator it = currentThread->space->l_threads.begin();

	while (it != currentThread->space->l_threads.end() && (tid != (*it)->tid))
	{
		++it;
	}
	// tid does not exist or an other thread wait for this thread
	if (it == currentThread->space->l_threads.end())
	{
		return -1;
	}
	else
	{
		th = *it;
		if(th->wait)
		{
			return -1;
		}
		else
		{
			th->wait = true;
			th->s_join->P();
			th->wait = false;
			th->s_join->V();
			return 0;
		}
	}
}

#endif

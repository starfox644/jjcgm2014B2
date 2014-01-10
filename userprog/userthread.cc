#ifdef CHANGED
#include "userthread.h"
#include "thread.h"
#include "system.h"
#include "machine.h"

/**
*    f : 		function address to execute in the MIPS processor
*    arg : 	argument address for the f function, f will be called with arg
*/
int do_UserThreadCreate(int f, int arg)
{
	int n;
	bool error = false;
	Thread *newThread = new Thread("test");

	// test the accessibility of the code and the argument
    error = !machine->ReadMem(f, sizeof(int), &n);
    if(!error)
    {
    	// accept arg 0 for passing NULL as argument
    	error = (arg == 0) || !machine->ReadMem(arg, sizeof(int), &n);
    	if(!error)
    	{
    		// test the accessibility of the stack
    		error = !machine->ReadMem(f+STACK_OFFSET*PageSize, sizeof(int), &n);
    	}
    }

    if(!error)
    {
		// the new thread shares the memory space with the current thread
		newThread->space = currentThread->space;
		// sets initial argument of the thread
		newThread->setInitArg(arg);

		// creation of the thread, init and positionning in the file
		// the new thread executes StartUserThread (saving register)
		newThread->Fork(StartUserThread, f);
    	return 0;
    }
    else
    {
    	delete newThread;
    	return -1;
    }
}

static void StartUserThread(int f)
{
	currentThread->space->InitRegisters();
	currentThread->space->RestoreState ();	// load page table register

	// copy the arg in register r4
	machine->WriteRegister(4, currentThread->getInitArg());
	// set PC
	machine->WriteRegister(PCReg, f);
	// set next PC
	machine->WriteRegister(NextPCReg, f+4);
	// set return address (none)
	machine->WriteRegister(31, -1);
	// set SP
	machine->WriteRegister(StackReg, f+2*PageSize);
	machine->Run ();		// jump to the user progam
}

void do_UserThreadExit()
{
	currentThread->Finish();
}

#endif

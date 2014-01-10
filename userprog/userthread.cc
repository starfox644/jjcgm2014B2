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
	// we create a function_t containing the function f and it’s arguments
	/*function_t fun;
	fun.f = f;
	fun.arg = arg;*/

	Thread *newThread = new Thread("test");
	// the new thread shares the memory space with the current thread
	newThread->space = currentThread->space;
	// sets initial argument of the thread
	newThread->setInitArg(arg);

	// creation of the thread, init and positionning in the file
	// the new thread executes StartUserThread (saving register)
	newThread->Fork(StartUserThread, f);
	return 0;
}

void StartUserThread(int f)
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
	machine->WriteRegister(StackReg, f+3*PageSize);
	machine->Run ();		// jump to the user progam
}

void do_UserThreadExit()
{
	currentThread->Finish();
}

#endif

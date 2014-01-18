// thread.cc 
//      Routines to manage threads.  There are four main operations:
//
//      Fork -- create a thread to run a procedure concurrently
//              with the caller (this is done in two steps -- first
//              allocate the Thread object, then call Fork on it)
//      Finish -- called when the forked procedure finishes, to clean up
//      Yield -- relinquish control over the CPU to another ready thread
//      Sleep -- relinquish control over the CPU, but thread is now blocked.
//              In other words, it will not run again, until explicitly 
//              put back on the ready queue.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "thread.h"
#include "switch.h"
#include "synch.h"
#include "system.h"

#ifdef CHANGED
#ifdef countNew
#include "countNew.h"
#endif
#endif

#define STACK_FENCEPOST 0xdeadbeef	// this is put at the top of the
// execution stack, for detecting
// stack overflows

#ifdef CHANGED
// the first thread has 0 as tid
int Thread::nextTid = 1;

#ifdef countNew
int Thread::nbNewThread = 0;
int Thread::nbThreadsCreated = 0;

int Thread::getNbNewThread() {
	return nbNewThread;
}

int Thread::getNbThreadsCreated() {
	return nbThreadsCreated;
}
#endif


#endif




//----------------------------------------------------------------------
// Thread::Thread
//      Initialize a thread control block, so that we can then call
//      Thread::Fork.
//
//      "threadName" is an arbitrary string, useful for debugging.
//----------------------------------------------------------------------

Thread::Thread (const char *threadName)
{
#ifdef CHANGED
	strncpy(name, threadName, 10);
#else
	name = threadName;
#endif
	stackTop = NULL;
	stack = NULL;
	status = JUST_CREATED;
#ifdef CHANGED
	process = NULL;
	tid = 0;
	wait = false;
	s_join = new Semaphore("semaphore for join", 1);
	isFinished = false;
	mustFinish = false;
#else

#ifdef USER_PROGRAM
	space = NULL;
	// FBT: Need to initialize special registers of simulator to 0
	// in particular LoadReg or it could crash when switching
	// user threads.
	int r;
	for (r=NumGPRegs; r<NumTotalRegs; r++)
		userRegisters[r] = 0;
#endif

#endif

#ifdef countNew
	nbNewThread++;
	nbThreadsCreated++;
	displayNew(nbNewThread, "Thread");
#endif
}

//----------------------------------------------------------------------
// Thread::~Thread
//      De-allocate a thread.
//
//      NOTE: the current thread *cannot* delete itself directly,
//      since it is still running on the stack that we need to delete.
//
//      NOTE: if this is the main thread, we can't delete the stack
//      because we didn't allocate it -- we got it automatically
//      as part of starting up Nachos.
//----------------------------------------------------------------------

Thread::~Thread ()
{
	DEBUG ('t', "Deleting thread \"%s\"\n", name);

#ifdef countNew
	nbNewThread--;
	displayNew(nbNewThread, "Thread");
#endif

	ASSERT (this != currentThread);
	if (stack != NULL)
		DeallocBoundedArray ((char *) stack, StackSize * sizeof (int));
#ifdef CHANGED
	delete s_join;
#endif
}

//----------------------------------------------------------------------
// Thread::Fork
//      Invoke (*func)(arg), allowing caller and callee to execute 
//      concurrently.
//
//      NOTE: although our definition allows only a single integer argument
//      to be passed to the procedure, it is possible to pass multiple
//      arguments by making them fields of a structure, and passing a pointer
//      to the structure as "arg".
//
//      Implemented as the following steps:
//              1. Allocate a stack
//              2. Initialize the stack so that a call to SWITCH will
//              cause it to run the procedure
//              3. Put the thread on the ready queue
//      
//      "func" is the procedure to run concurrently.
//      "arg" is a single argument to be passed to the procedure.
//----------------------------------------------------------------------

void
Thread::Fork (VoidFunctionPtr func, int arg)
{
	DEBUG ('t', "Forking thread \"%s\" with func = 0x%x, arg = %d\n",
			name, (int) func, arg);

#ifdef CHANGED
	// blocks the semaphore for other threads to wait it
	s_join->P();
#endif
	StackAllocate (func, arg);

#ifdef USER_PROGRAM

	// LB: The addrspace should be tramsitted here, instead of later in
	// StartProcess, so that the pageTable can be restored at
	// launching time. This is crucial if the thread is launched with
	// an already running program, as in the "fork" Unix system call.

	// LB: Observe that currentThread->space may be NULL at that time.
/*#ifndef step4
	this->space = currentThread->space;
#endif*/

#endif // USER_PROGRAM

	IntStatus oldLevel = interrupt->SetLevel (IntOff);
	scheduler->ReadyToRun (this);	// ReadyToRun assumes that interrupts
	// are disabled!
	(void) interrupt->SetLevel (oldLevel);
}

//----------------------------------------------------------------------
// Thread::CheckOverflow
//      Check a thread's stack to see if it has overrun the space
//      that has been allocated for it.  If we had a smarter compiler,
//      we wouldn't need to worry about this, but we don't.
//
//      NOTE: Nachos will not catch all stack overflow conditions.
//      In other words, your program may still crash because of an overflow.
//
//      If you get bizarre results (such as seg faults where there is no code)
//      then you *may* need to increase the stack size.  You can avoid stack
//      overflows by not putting large data structures on the stack.
//      Don't do this: void foo() { int bigArray[10000]; ... }
//----------------------------------------------------------------------

void
Thread::CheckOverflow ()
{
	if (stack != NULL)
#ifdef HOST_SNAKE		// Stacks grow upward on the Snakes
		ASSERT (stack[StackSize - 1] == STACK_FENCEPOST);
#else
	ASSERT (*stack == (int) STACK_FENCEPOST);
#endif
}

//----------------------------------------------------------------------
// Thread::Finish
//      Called by ThreadRoot when a thread is done executing the 
//      forked procedure.
//
//      NOTE: we don't immediately de-allocate the thread data structure 
//      or the execution stack, because we're still running in the thread 
//      and we're still on the stack!  Instead, we set "threadToBeDestroyed", 
//      so that Scheduler::Run() will call the destructor, once we're
//      running in the context of a different thread.
//
//      NOTE: we disable interrupts, so that we don't get a time slice 
//      between setting threadToBeDestroyed, and going to sleep.
//----------------------------------------------------------------------

//
void
Thread::Finish ()
{
	(void) interrupt->SetLevel (IntOff);
	ASSERT (this == currentThread);
	DEBUG ('t', "Finishing thread \"%s\"\n", getName ());
#ifdef CHANGED
	// release the semaphore for threads which are waiting the end
	s_join->V();
	if(currentThread->isMainThread())
	{
		// the main thread is the last to finish, it must be deleted
		threadToBeDestroyed = currentThread;
	}

	if (currentProcess == NULL) {
		Printf("currentProcess == NULL, exit\n");
		Exit(0);
	}

	AddrSpace* space = currentProcess->getAddrSpace();
	if(space != NULL)
	{
		// free the stack of the thread
#ifdef step4
		if(!currentThread->isMainThread())
		{
			space->freeThreadStack(userStackAddr);
		}
#else
		if(!currentThread->isMainThread())
			space->addAvailableStackAddress(userStackAddr);
#endif
	}
#endif
#ifndef CHANGED
	// LB: Be careful to guarantee that no thread to be destroyed
	// is ever lost
	ASSERT (threadToBeDestroyed == NULL);
	// End of addition
	threadToBeDestroyed = currentThread;
#endif
	Sleep ();			// invokes SWITCH
	// not reached
}

//----------------------------------------------------------------------
// Thread::Yield
//      Relinquish the CPU if any other thread is ready to run.
//      If so, put the thread on the end of the ready list, so that
//      it will eventually be re-scheduled.
//
//      NOTE: returns immediately if no other thread on the ready queue.
//      Otherwise returns when the thread eventually works its way
//      to the front of the ready list and gets re-scheduled.
//
//      NOTE: we disable interrupts, so that looking at the thread
//      on the front of the ready list, and switching to it, can be done
//      atomically.  On return, we re-set the interrupt level to its
//      original state, in case we are called with interrupts disabled. 
//
//      Similar to Thread::Sleep(), but a little different.
//----------------------------------------------------------------------

void
Thread::Yield ()
{
	Thread *nextThread;
	IntStatus oldLevel = interrupt->SetLevel (IntOff);

	ASSERT (this == currentThread);
	DEBUG ('t', "Yielding thread \"%s\"\n", getName ());

	nextThread = scheduler->FindNextToRun ();
	if (nextThread != NULL)
	{
		scheduler->ReadyToRun (this);
		scheduler->Run (nextThread);
	}
	(void) interrupt->SetLevel (oldLevel);
}

//----------------------------------------------------------------------
// Thread::Sleep
//      Relinquish the CPU, because the current thread is blocked
//      waiting on a synchronization variable (Semaphore, Lock, or Condition).
//      Eventually, some thread will wake this thread up, and put it
//      back on the ready queue, so that it can be re-scheduled.
//
//      NOTE: if there are no threads on the ready queue, that means
//      we have no thread to run.  "Interrupt::Idle" is called
//      to signify that we should idle the CPU until the next I/O interrupt
//      occurs (the only thing that could cause a thread to become
//      ready to run).
//
//      NOTE: we assume interrupts are already disabled, because it
//      is called from the synchronization routines which must
//      disable interrupts for atomicity.   We need interrupts off 
//      so that there can't be a time slice between pulling the first thread
//      off the ready list, and switching to it.
//----------------------------------------------------------------------
void
Thread::Sleep ()
{
	Thread *nextThread;

	ASSERT (this == currentThread);
	ASSERT (interrupt->getLevel () == IntOff);

	DEBUG ('t', "Sleeping thread \"%s\"\n", getName ());

	status = BLOCKED;
	while ((nextThread = scheduler->FindNextToRun ()) == NULL)
		interrupt->Idle ();	// no one to run, wait for an interrupt

	scheduler->Run (nextThread);	// returns when we've been signalled
}

//----------------------------------------------------------------------
// ThreadFinish, InterruptEnable, ThreadPrint
//      Dummy functions because C++ does not allow a pointer to a member
//      function.  So in order to do this, we create a dummy C function
//      (which we can pass a pointer to), that then simply calls the 
//      member function.
//----------------------------------------------------------------------

static void
ThreadFinish ()
{
	currentThread->Finish ();
}
static void
InterruptEnable ()
{
	interrupt->Enable ();
}

// LB: This function has to be called on starting  a new thread to set
// up the pagetable correctly. This was missing from the original
// version. Credits to Clement Menier for finding this bug!

void 
SetupThreadState ()
{

	// LB: Similar to the second part of Scheduler::Run. This has to be
	// done each time a thread is scheduled, either by SWITCH, or by
	// getting created.

	if (threadToBeDestroyed != NULL)
	{
		delete threadToBeDestroyed;
		threadToBeDestroyed = NULL;
	}

#ifdef USER_PROGRAM

	// LB: Now, we have to simulate the RestoreUserState/RestoreState
	// part of Scheduler::Run

	// Be very careful! We have no information about the thread which is
	// currently running at the time this function is called. Actually,
	// there is no reason why the running thread should have the same
	// pageTable as the thread just being created.

	// This is definitely the case as soon as several *processes* are
	// running together.
#ifdef CHANGED
	AddrSpace* space = currentProcess->getAddrSpace();
	if (space != NULL)
    {				// if there is an address space
	  currentThread->RestoreUserState ();	// to restore, do it.
	  space->RestoreState ();
    }
#else
	if (currentThread->space != NULL)
	{				// if there is an address space
		// LB: Actually, the user state is void at that time. Keep this
		// action for consistency with the Scheduler::Run function
		currentThread->RestoreUserState ();	// to restore, do it.
		currentThread->space->RestoreState ();
	}
#endif // changed
#endif // USER_PROGRAM

	// LB: The default level for interrupts is IntOn.
	InterruptEnable ();

}

// End of addition

void
ThreadPrint (int arg)
{
	Thread *t = (Thread *) arg;
	t->Print ();
}

#ifdef CHANGED
void Thread::setName(char* newName)
{
	strncpy(name, newName, 10);
}
#endif

//----------------------------------------------------------------------
// Thread::StackAllocate
//      Allocate and initialize an execution stack.  The stack is
//      initialized with an initial stack frame for ThreadRoot, which:
//              enables interrupts
//              calls (*func)(arg)
//              calls Thread::Finish
//
//      "func" is the procedure to be forked
//      "arg" is the parameter to be passed to the procedure
//----------------------------------------------------------------------

void
Thread::StackAllocate (VoidFunctionPtr func, int arg)
{
	stack = (int *) AllocBoundedArray (StackSize * sizeof (int));

#ifdef HOST_SNAKE
	// HP stack works from low addresses to high addresses
	stackTop = stack + 16;	// HP requires 64-byte frame marker
	stack[StackSize - 1] = STACK_FENCEPOST;
#else
	// i386 & MIPS & SPARC stack works from high addresses to low addresses
#ifdef HOST_SPARC
	// SPARC stack must contains at least 1 activation record to start with.
	stackTop = stack + StackSize - 96;
#else // HOST_MIPS  || HOST_i386
	stackTop = stack + StackSize - 4;	// -4 to be on the safe side!
#ifdef HOST_i386
	// the 80386 passes the return address on the stack.  In order for
	// SWITCH() to go to ThreadRoot when we switch to this thread, the
	// return addres used in SWITCH() must be the starting address of
	// ThreadRoot.
	*(--stackTop) = (int) ThreadRoot;
#endif
#endif // HOST_SPARC
	*stack = STACK_FENCEPOST;
#endif // HOST_SNAKE

	machineState[PCState] = (int) ThreadRoot;

	// LB: It is not sufficient to enable interrupts!
	// A more complex function has to be called here...
	// machineState[StartupPCState] = (int) InterruptEnable;
	machineState[StartupPCState] = (int) SetupThreadState;
	// End of modification

	machineState[InitialPCState] = (int) func;
	machineState[InitialArgState] = arg;
	machineState[WhenDonePCState] = (int) ThreadFinish;
}

#ifdef USER_PROGRAM
#include "machine.h"

//----------------------------------------------------------------------
// Thread::SaveUserState
//      Save the CPU state of a user program on a context switch.
//
//      Note that a user program thread has *two* sets of CPU registers -- 
//      one for its state while executing user code, one for its state 
//      while executing kernel code.  This routine saves the former.
//----------------------------------------------------------------------

void
Thread::SaveUserState ()
{
	for (int i = 0; i < NumTotalRegs; i++)
		userRegisters[i] = machine->ReadRegister (i);
}

//----------------------------------------------------------------------
// Thread::RestoreUserState
//      Restore the CPU state of a user program on a context switch.
//
//      Note that a user program thread has *two* sets of CPU registers -- 
//      one for its state while executing user code, one for its state 
//      while executing kernel code.  This routine restores the former.
//----------------------------------------------------------------------

void
Thread::RestoreUserState ()
{
	for (int i = 0; i < NumTotalRegs; i++)
		machine->WriteRegister (i, userRegisters[i]);
}

#ifdef CHANGED

void Thread::setIsSyscall(bool b) {
	isSyscall = b;
}

bool Thread::getIsSyscall() {
	return isSyscall;
}

void Thread::setInitArg(int arg)
{
	initArg = arg;
}

int Thread::getInitArg()
{
	return initArg;
}

bool Thread::isMainThread()
{
	// stack is always NULL for the main thread
	return (tid == 0);
}

void Thread::setThreadReturn(int th_ret)
{
	thread_return = th_ret;
}

int Thread::getThreadReturn()
{
	return thread_return;
}

int Thread::getTid()
{
	if(nextTid <= INT_MAX)
	{
		return nextTid++;
	}
	else
	{
		return -1;
	}
}

void Thread::Print ()
{
	Printf ("%s, ", name);
}

#endif
#endif



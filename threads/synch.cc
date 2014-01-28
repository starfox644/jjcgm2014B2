// synch.cc 
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks 
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore (const char *debugName, int initialValue)
{
	name = debugName;
	value = initialValue;
	queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore ()
{
	delete queue;
}

#ifdef CHANGED
/**
 *	Return the semaphore id
 */
int Semaphore::getId()
{
	return this->id;
}

/**
 *	Set the semaphore id
 */
void Semaphore::setId(int idSem)
{
	id = idSem;
}
#endif // CHANGED

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------

void Semaphore::P ()
{
	IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts

	while (value == 0)
	{				// semaphore not available
		queue->Append ((void *) currentThread);	// so go to sleep
		currentThread->Sleep ();
	}
	value--;			// semaphore available,
	// consume its value

	(void) interrupt->SetLevel (oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V ()
{
	Thread *thread;
	IntStatus oldLevel = interrupt->SetLevel (IntOff);

	thread = (Thread *) queue->Remove ();
	if (thread != NULL)		// make thread ready, consuming the V immediately
		scheduler->ReadyToRun (thread);
	value++;

	(void) interrupt->SetLevel (oldLevel);
}
// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock (const char *debugName)
{
#ifdef CHANGED
	sem = new Semaphore(debugName, 1);
	holder = NULL;
#endif
}

Lock::~Lock ()
{
#ifdef CHANGED
	delete sem;
#endif
}

void Lock::Acquire ()
{
#ifdef CHANGED
	sem->P();
	holder = currentThread;
#endif
}

void Lock::Release ()
{
#ifdef CHANGED
	if (isHeldByCurrentThread())
	{
		holder = NULL;
		sem->V();
	}
	else
		printf("[Lock::Release] Erreur - Ce lock n'est pas pris par le thread courant.\n");
#endif
}

bool Lock::isHeldByCurrentThread ()
{
#ifdef CHANGED
	return holder == currentThread;
#endif
}

Condition::Condition (const char *debugName)
{
	name = debugName;
#ifdef NETWORK
	semProtList = new Semaphore("Semaphore protection Liste",1);
	l_semCond = new List();
#endif
}

Condition::~Condition ()
{
#ifdef NETWORK
	delete semProtList;
	delete l_semCond;
#endif
}

void Condition::Wait (Lock * conditionLock)
{
#ifdef NETWORK
	if (conditionLock->isHeldByCurrentThread())
	{
		Semaphore *semCond = new Semaphore("condWait", 0);
//		printf("[Condition::Wait] debut fonction\n"); // TODO
		// On ajoute le semaphore a notre liste
//		semProtList->P();
		l_semCond->Append(semCond);
//		semProtList->V();
		// On commence par liberer notre lock
		conditionLock->Release();
		// On prend notre semaphore pour attendre
		semCond->P();
		conditionLock->Acquire(); // on recupere notre lock
//		printf("[Condition::Wait] fin fonction\n"); // TODO
	}
	else
	{
		printf("[Condition::Wait] Erreur - Ce lock n'est pas pris par le thread courant.\n");
		Exit(-1);
	}
#endif//network
}

void Condition::Signal (Lock * conditionLock)
{
#ifdef NETWORK
	if (conditionLock->isHeldByCurrentThread())
	{
		Semaphore *semCond;
//		printf("[Condition::Signal] debut fonction\n"); // TODO
//		semProtList->P();
		semCond = (Semaphore*) l_semCond->Remove();
		if (semCond != NULL)
		{
			//on libere notre semaphore des conditions
//			semProtList->V();
			semCond->V();
		}
	}
	else
	{
		printf("[Condition::Signal] Erreur - Ce lock n'est pas pris par le thread courant.\n");
		Exit(-1);
	}
#endif //network
}

void Condition::Broadcast (Lock * conditionLock)
{
#ifdef NETWORK
	if (conditionLock->isHeldByCurrentThread())
	{
		Semaphore *semCond;
		while (!l_semCond->IsEmpty())
		{
//			semProtList->P();
			semCond = (Semaphore*) l_semCond->Remove();
			//on libere notre semaphore des conditions
//			semProtList->V();
			semCond->V();
		}
	}
	else
	{
		printf("[Condition::Signal] Erreur - Ce lock n'est pas pris par le thread courant.\n");
		Exit(-1);
	}
#endif
}

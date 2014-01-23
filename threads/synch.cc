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
#ifdef NETWORK
	inUse = false;
	inListCond = false;
#endif
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

void
Semaphore::P ()
{
	IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts
#ifdef NETWORK
	while (value == 0 && inListCond == true)
#else
		while (value == 0)
#endif
		{				// semaphore not available
			queue->Append ((void *) currentThread);	// so go to sleep
			currentThread->Sleep ();
		}
	value--;			// semaphore available,
	// consume its value
#ifdef NETWORK
	inUse = true;
#endif
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
#ifdef NETWORK
	inUse = false;
#endif
	(void) interrupt->SetLevel (oldLevel);
}
#ifdef NETWORK
bool Semaphore::getInUse(){
	return inUse;
}
void Semaphore::setInUse(bool b){
	inUse = b;
}
bool Semaphore::getInListCond(){
	return inListCond;
}
void Semaphore::setInListCond(bool b){
	inListCond = b;
}
#endif
// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock (const char *debugName)
{
#ifdef CHANGED
	sem = new Semaphore(debugName, 1);
#endif
}

Lock::~Lock ()
{
#ifdef CHANGED
	delete sem;
#endif
}
void
Lock::Acquire ()
{
#ifdef CHANGED
	sem->P();
#endif
}
void
Lock::Release ()
{
#ifdef CHANGED
	sem->V();
#endif
}

Condition::Condition (const char *debugName)
{
	name = debugName;
#ifdef NETWORK
	cond = new Semaphore("Semaphore Condition",1);
#endif
}

Condition::~Condition ()
{
#ifdef NETWORK
	std::list<Lock*>::iterator it;
	//liberation de la liste
	for(it = l_condLock.begin() ; it != l_condLock.end() ; ++it)
	{
		delete (*it);
	}
	delete cond; // liberation de la semaphore
#endif
}
int Condition::Wait (Lock * conditionLock)
{
#ifdef NETWORK
	cond->P();
	conditionLock->sem->V(); //on libere la semaphore du lock et on l'ajoute a notre liste
	if(conditionLock->sem->getInUse() == true && conditionLock->sem->getInListCond() == false){//si semaphore utilisé mais pas dans la liste
		cond->V();
		return 0;
	}else if(conditionLock->sem->getInUse() == true && conditionLock->sem->getInListCond() == true){		//si semaphore utlisé mais dans la liste
		std::list<Lock*>::iterator it=l_condLock.begin();
		while (it != l_condLock.end() && (*it) != conditionLock)
			it++;
		// If lock not found, return -1 : error
		if ((*it) != conditionLock){
			cond->V();
			return -1;
		}else{// Else, remove it
			nbCond--;
			conditionLock->sem->setInListCond(false);
			l_condLock.erase(it);
		}
	}else if(conditionLock->sem->getInListCond() == true){//si la semaphore et non utilisé mais deja dans la liste
		cond->V();
		return 0;
	}else{//si semaphore non utilisé et pas dans la liste
		conditionLock->sem->setInListCond(true);
		l_condLock.push_back(conditionLock);
		nbCond++;
	}
	cond->V();
#endif//network
	return 0;
}

int Condition::Signal (Lock * conditionLock)
{
#ifdef NETWORK
	cond->P();
	std::list<Lock*>::iterator it=l_condLock.begin();
	while (it != l_condLock.end() && (*it) != conditionLock)
		it++;
	// If lock not found, return -1 : error
	if ((*it) != conditionLock){
		cond->V();
		return -1;
		// Else, remove it
	}else{
		nbCond--;
		conditionLock->sem->setInListCond(false);
		l_condLock.erase(it);
		cond->V();
		return 0;
	}
#endif //network
	return 0;
}
int Condition::Broadcast (Lock * conditionLock)
{
#ifdef NETWORK
	cond->P();
	std::list<Lock*>::iterator it=l_condLock.begin();
	while (it != l_condLock.end() && (*it) != conditionLock){
		it++;
		nbCond--;
		conditionLock->sem->setInListCond(false);
		l_condLock.erase(it);
	}
	cond->V();
#endif
	return 0;
}

// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#ifdef CHANGED
class Semaphore;
class Thread;
#include <list>
#endif

#ifdef CHANGED
#define UserStackSize		2048	// increase this as necessary!
#else
#define UserStackSize		1024	// increase this as necessary!
#endif

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

#ifdef CHANGED
    /**
     *  add a thread to this address space
     */
    void addThread(Thread *th);

    /**
     *  remove a thread from this address space
     */
    void removeThread(Thread *th);

    /**
     * 	returns an initial stack pointer available for a new thread and removes it
     * 	or returns -1 if it's impossible to add a new stack in the address space
     */
    int popAvailableStackPointer();

    /**
     * 	add a stackAddr to the list of available stack address
     * 	this stack address must be in the address space
     */
	void addAvailableStackAddress(int stackAddr);

    /**
     * 	returns the number of user threads, without the main thread
     */
    int getNbThreads();

    // locks access to the nbThreads variable
    Semaphore *s_nbThreads;
    // locks the main thread while the others are finishing
    Semaphore *s_exit;
    bool attente;
    // threads of the address space
    std::list<Thread*> l_threads;

#endif

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
#ifdef CHANGED
    // address where the memory available for threads' stacks begins
    int beginThreadsStackSpace;
    // address where the memory available for threads' stacks ends
    int endThreadsStackSpace;
    // number of threads in execution without the main thread
    int nbThreads;
    Semaphore* s_stackList;
    // list of available stack address in the address space for the threads
    std::list<int> l_availableStackAddress;
    // number max of threads depending on memory for the stacks
    int maxThreads;

    void initAvailableStackPointers();
#endif
};

#endif // ADDRSPACE_H

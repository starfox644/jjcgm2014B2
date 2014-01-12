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
     * 	or -1 if it's impossible to add a new stack in the address space
     */
    int popAvailableStackPointer();

    /**
     * 	returns the number of user threads, without the main thread
     */
    int getNbThreads();

    Semaphore *s_nbThreads;
    Semaphore *s_exit;
    bool attente;

    /**
     * Add newSem to semList and allocate it a unique modifier
     */
    int addSemaphore(int initValue);

    /**
     * Remove a semaphore from the list based on his identifier.
     * If the identifier is valid, the semaphore is destroyed.
     * If not, the function returns -1.
     */
    int removeSemaphore(int id);

    /**
     * Return the semaphore identified by id, or NULL if it doesn't exist
     */
    Semaphore* getSemaphore(int id);

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
    // number of semaphore created
    int nbSem;

    // Semaphore list : needed to give an unique identifier for user semaphores
    std::list<Semaphore*> semList;
    // list of available stack address in the address space for the threads
    std::list<int> l_availableStackAddress;
    // number max of threads depending on memory for the stacks
    int maxThreads;

    void initAvailableStackPointers();
#endif //CHANGED
};

#endif // ADDRSPACE_H

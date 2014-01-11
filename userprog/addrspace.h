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
#endif

#ifdef CHANGED
#define UserStackSize		4096	// increase this as necessary!
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
    void addThread();

    /**
     *  remove a thread from this address space
     */
    void removeThread();

    /**
     * 	returns the number of user threads, without the main thread
     */
    int getNbThreads();

    Semaphore *s_nbThreads;
    Semaphore *s_exit;
    bool attente;
#endif

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
#ifdef CHANGED
    // number of threads in execution without the main thread
    int nbThreads;
#endif
};

#endif // ADDRSPACE_H

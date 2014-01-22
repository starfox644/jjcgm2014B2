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
class threadManager;
#include "noff.h"
#include <list>

#define UserStackSize		4096	// increase this as necessary!
#define UserStackPages		UserStackSize / PageSize;

#else
#define UserStackSize		1024	// increase this as necessary!
#endif

#ifdef step4
#include "addrSpaceAllocator.h"
#endif

class AddrSpace
{
  public:

#ifdef CHANGED
#ifdef countNew
	// to count number of allocations
	static int nbNewAddrspace;
	static int nbAddrspaceCreated;

	static 	int getNbNewAddrspace();
	static int getNbAddrspaceCreated();
#endif
#endif

#ifdef step4
	/**
	 * 	STEP 4 constructor
	 * 	We don't allocate virtual memory here
	 * 	This is done in loadInitialSections which have to be called after
	 * 	This permits to test allocation errors
	 */
	AddrSpace();
#else
	/**
	 *  Constructor for all the steps except step 4
	 * 	Allocate all the needed structures and load the program
	 */
    AddrSpace (OpenFile * executable);
#endif
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

#ifdef CHANGED

#ifdef step4
    /**
     *  allocate memory needed for the code, data and stack sections
     */
    bool loadInitialSections(OpenFile * executable);

    /** manages the available virtual memory */
    AddrSpaceAllocator* addrSpaceAllocator;

#endif

    /**
     * 	returns an initial stack pointer available for a new thread and removes it
     * 	or returns -1 if it's impossible to add a new stack in the address space
     * 	Used for step3
     */
    int popAvailableStackPointer();

    /**
     * 	add a stackAddr to the list of available stack address
     * 	this stack address must be in the address space
     * 	Used for step3
     */
	void addAvailableStackAddress(unsigned int stackAddr);

    // locks the main thread while the others are finishing
    Semaphore *s_exit;

#ifdef step4
    static void ReadAtVirtual(OpenFile* executable, int virtualaddr, int numBytes, int position,
	TranslationEntry *pageTable,unsigned numPages);

    /**
     * Allocate length bytes at virtualAddr in the address space.
     * Associate it with frames in physical memory.
     * The part allocated begins at the begining of the page of virtualAddr
     * and ends with the number of pages needed for the length.
     * If write is set, then the pages are allowed for writing.
     * if there is no more physical frames available, returns true
     * else returns false
     * If some pages of the needed part are already mapped, there is no
     * error and the associated frames are used for the new mapping.
     */
    bool mapMem(int virtualAddr, int length, bool write);

    /**
     * 	Release nbFrames of physical memory beginning at beginPageIndex
     * 	returns false if at least one page isn't allocated
     */
    bool unMapMem(unsigned int beginPageIndex, unsigned int nbFrames);

    /**
	 *  Print the index of physical frames associated to the pages.
	*/
    void printMapping(unsigned int max);

    /**
     *  Allocate length bytes in the virtual memory.
     *  The address is chosen by the addrSpaceAllocator.
     *  Return -1 in case of failure, else the address of the mapping.
     */
    int mmap(int length);

    /**
     * 	Free the memory allocated at the given address.
     * 	Return -1 if the address wasn't allocated else 0.
     */
    int unmap(int addr);

//
//	FOR THREADS OF STEP 4
//	We use an allocator of virtual memory for the stacks
//

    /**
     *	Allocate a stack for a thread in the virtual memory,
     *	and associate it with physical frames.
     *	Return -1 on error
     */
	int allocThreadStack();

	/**
	 *	Release a stack which was allocated for a thread in virtual memory.
	 */
	void freeThreadStack(unsigned int stackAddr);

	/**
	 * 	Set the access right (read only or writing) of a group of nbPages pages,
	 * 	begining at beginPages.
	 */
	bool setAccessRight(unsigned int beginPage, unsigned int nbPages, bool readOnly);

#endif

#endif

      TranslationEntry * pageTable;	// Assume linear page table translation
  private:

    unsigned int numPages;	// Number of pages in the virtual address space
#ifdef CHANGED
    // address where the memory available for threads' stacks begins
    unsigned int beginThreadsStackSpace;
    // address where the memory available for threads' stacks ends
    unsigned int endThreadsStackSpace;

#ifdef step4
    // number of pages needed for a stack
    int nbPagesUserStack;

    /**
     *	Map code and data of executable using the noff header.
     *	Returns false if this is impossible (no ressources)
     */
    bool mapExecutable(NoffHeader noffH, OpenFile * executable);

#endif
	// number max of threads depending on memory for the stacks
	// the main thread is not included in this number
	int maxThreads;

//
//	FOR THREADS OF STEP 3
//	We use a list of available stack address
//

	// semaphore for locking the access of the stack list
    Semaphore* s_stackList;
    // list of available stack address in the address space for the threads
    std::list<int> l_availableStackAddress;

    /**
     * 	init the stack list with the initial stack pointers
     * 	for the additionnal thread's stacks
     */
    void initAvailableStackPointers();

#endif //CHANGED
};

#endif // ADDRSPACE_H

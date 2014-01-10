// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#ifdef CHANGED
#include <string>
class FrameProvider;
#endif
#include <strings.h>		/* for bzero */

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
	noffH->noffMagic = WordToHost (noffH->noffMagic);
	noffH->code.size = WordToHost (noffH->code.size);
	noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
	noffH->initData.size = WordToHost (noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost (noffH->uninitData.size);
	noffH->uninitData.virtualAddr =
			WordToHost (noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
	NoffHeader noffH;
	unsigned int i, size;

#ifdef CHANGED
	unsigned int availableStackSize;
	nbSem = 0;
	nbThreads = 0;
	attente = false;
	s_exit = new Semaphore("exit semaphore", 0);
	s_nbThreads = new Semaphore("nbThread semaphore", 1);
	s_stackList = new Semaphore("stack list semaphore", 1);
	s_userJoin = new Semaphore("user join semaphore", 1);
#endif

	executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
	if ((noffH.noffMagic != NOFFMAGIC) &&
			(WordToHost (noffH.noffMagic) == NOFFMAGIC))
		SwapHeader (&noffH);
	ASSERT (noffH.noffMagic == NOFFMAGIC);
	// how big is address space?

#ifdef CHANGED
	DEBUG ('a', "Executable informations :\n");
	DEBUG('a', "code size : %d\n", noffH.code.size);
	DEBUG('a', "init data size : : %d\n", noffH.initData.size);
	DEBUG('a', "uninit data size : : %d\n", noffH.uninitData.size);
	// we use all the memory for the process
	size = MemorySize - PageSize;
	// the available stack space begin after the main thread stack
	beginThreadsStackSpace = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
	// to leave room for the stack
	numPages = divRoundUp (size, PageSize);
	size = numPages * PageSize;
	availableStackSize = (size - beginThreadsStackSpace) - 1;
	// the main thread is not included in this number
	maxThreads = availableStackSize / UserStackSize;


	// the stacks space ends with the memory
	endThreadsStackSpace = MemorySize - 1;
	initAvailableStackPointers();

	/*printf("endThreadsStackSpace : %d\n", endThreadsStackSpace);
    printf("Address Space size : %d\n", MemorySize);
    printf("beginThreadsStackSpace : %d\n", beginThreadsStackSpace);
    printf("availableStackSize : %d\n", availableStackSize);
    printf("nombre max de threads : %d\n", maxThreads);
    printf("nb pages : %d\n", numPages);*/
#else
	size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
	// to leave room for the stack
	numPages = divRoundUp (size, PageSize);
	size = numPages * PageSize;
#endif

	ASSERT (numPages <= NumPhysPages);	// check we're not trying
	// to run anything too big --
	// at least until we have
	// virtual memory

	DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
			numPages, size);
	// first, set up the translation
	pageTable = new TranslationEntry[numPages];
	for (i = 0; i < numPages; i++)
	{
		pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
#ifdef step4
		pageTable[i].physicalPage = frameProvider->GetEmptyFrame();
#else
		pageTable[i].physicalPage = i;
#endif
		pageTable[i].valid = TRUE;
		pageTable[i].use = FALSE;
		pageTable[i].dirty = FALSE;
		pageTable[i].readOnly = FALSE;	// if the code segment was entirely on
		// a separate page, we could set its
		// pages to be read-only
	}

	// zero out the entire address space, to zero the unitialized data segment
	// and the stack segment
	bzero (machine->mainMemory, size);

#ifdef step4
	// then, copy in the code and data segments into memory
	if (noffH.code.size > 0)
	{
		DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
				noffH.code.virtualAddr, noffH.code.size);
		ReadAtVirtual (executable, noffH.code.virtualAddr,
				noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
	}
	if (noffH.initData.size > 0)
	{
		DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
				noffH.initData.virtualAddr, noffH.initData.size);
		ReadAtVirtual(executable, noffH.initData.virtualAddr,
						noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
	}
#else

	// then, copy in the code and data segments into memory
		if (noffH.code.size > 0)
		{
			DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
					noffH.code.virtualAddr, noffH.code.size);
			executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
					noffH.code.size, noffH.code.inFileAddr);
		}
		if (noffH.initData.size > 0)
		{
			DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
					noffH.initData.virtualAddr, noffH.initData.size);
			executable->ReadAt (&
					(machine->mainMemory
							[noffH.initData.virtualAddr]),
							noffH.initData.size, noffH.initData.inFileAddr);
		}
#endif
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
	// LB: Missing [] for delete
	// delete pageTable;
	delete [] pageTable;
	// End of modification
#ifdef CHANGED
	delete s_nbThreads;
	delete s_exit;
	delete s_stackList;
	deleteThreads();

	// Suppression des semaphores
	std::list<Semaphore*>::iterator it = semList.begin();
	std::list<Semaphore*>::iterator itDel;
	while (it != semList.end())
	{
		itDel = it;
		it++;
		delete *itDel;
	}
#endif
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
	int i;

	for (i = 0; i < NumTotalRegs; i++)
		machine->WriteRegister (i, 0);

	// Initial program counter -- must be location of "Start"
	machine->WriteRegister (PCReg, 0);

	// Need to also tell MIPS where next instruction is, because
	// of branch delay possibility
	machine->WriteRegister (NextPCReg, 4);

	// Set the stack register to the end of the address space, where we
	// allocated the stack; but subtract off a bit, to make sure we don't
	// accidentally reference off the end!
#ifdef CHANGED
	machine->WriteRegister (StackReg, beginThreadsStackSpace);
	DEBUG ('a', "Initializing stack register to %d\n",
			numPages * PageSize - 16);
#else
	machine->WriteRegister (StackReg, numPages * PageSize - 16);
	DEBUG ('a', "Initializing stack register to %d\n",
			numPages * PageSize - 16);
#endif
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;
}

#ifdef CHANGED
void AddrSpace::addThread(Thread *th)
{
	nbThreads++;
	// add the new thread in threads list
	l_threads.push_back(th);
}

void AddrSpace::removeThread(Thread *th)
{
	nbThreads--;
}

int AddrSpace::getNbThreads()
{
	return nbThreads;
}

void AddrSpace::deleteThreads()
{
	std::list<Thread*>::iterator it;
	for(it = l_threads.begin() ; it != l_threads.end() ; it++)
	{
		delete *it;
	}
}

/**
 * Add newSem to semList, give it a unique modifier and return the id
 */
int AddrSpace::addSemaphore(int initValue)
{
	// unique name based on identifier
	const char *name = "UserSem" + nbSem;
	Semaphore *sem = new Semaphore (name, initValue);
	// Set the semaphore id
	sem->setId(nbSem);
	nbSem++;
	// Add it to the list
	semList.push_back(sem);
	return sem->getId();
}

/**
 * Remove a semaphore from the list based on his identifier.
 * If the identifier is valid, the semaphore is destroyed.
 * If not, the function returns -1.
 */
int AddrSpace::removeSemaphore(int id)
{
	// iterator to find the semaphore in the list
	std::list<Semaphore*>::iterator it=semList.begin();
	while (it != semList.end() && (*it)->id != id)
		it++;
	// If semaphore not found, return -1 : error
	if ((*it)->id != id)
		return -1;
	// Else, remove it
	else
	{
		semList.erase(it);
		return 0;
	}
}

/**
 * Return the semaphore identified by id, or NULL if it doesn't exist
 */
Semaphore* AddrSpace::getSemaphore(int id)
{
	// iterator to find the semaphore in the list
	std::list<Semaphore*>::iterator it=semList.begin();
	while (it != semList.end() && (*it)->id != id)
		it++;
	// If semaphore not found, return -1 : error
	if ((*it)->id != id)
		return NULL;
	// Else, return it
	else
		return (Semaphore*)(*it);
}

/**
 * 	returns an initial stack pointer available for a new thread
 * 	or -1 if it's impossible to add a new stack in the address space
 */
int AddrSpace::popAvailableStackPointer()
{
	int return_value;
	s_stackList->P();
	if(l_availableStackAddress.size() == 0)
	{
		return_value = -1;
	}
	else
	{
		return_value = l_availableStackAddress.front();
		l_availableStackAddress.pop_front();
	}
	s_stackList->V();
	return return_value;
}

void AddrSpace::addAvailableStackAddress(unsigned int stackAddr)
{
	ASSERT(stackAddr < (numPages*PageSize) && stackAddr >= beginThreadsStackSpace);
	s_stackList->P();
	l_availableStackAddress.push_back(stackAddr);
	s_stackList->V();
}

void AddrSpace::initAvailableStackPointers()
{
	int addr = beginThreadsStackSpace + UserStackSize;
	s_stackList->P();
	for(int i = 0 ; i < maxThreads ; i++)
	{
		//printf("insertion de %d\n", addr);
		l_availableStackAddress.push_back(addr);
		addr += UserStackSize;
	}
	s_stackList->V();
}

#ifdef step4
void AddrSpace::ReadAtVirtual(OpenFile* executable, int virtualaddr, int numBytes, int position,
		TranslationEntry *pageTable,unsigned numPages)
{
	char* buffer = new char[numBytes];
	int i, nbRead;
	// lecture dans la memoire virtuelle a la position donnee puis le stocke dans le buffer
	nbRead = executable->ReadAt(buffer, numBytes, position);
	// charge la table des pages du processeur
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;

	// copie du buffer en memoire a l’aide de WriteMem
	for (i = 0 ; i < nbRead ; i++)
	{
		machine->WriteMem(virtualaddr+i,1,buffer[i]);
	}
}
#endif

#endif

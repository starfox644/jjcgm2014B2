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
#ifndef CHANGED
#include "noff.h"
#endif

#ifdef CHANGED
#include <string>
#endif

#include <strings.h>		/* for bzero */

#ifdef step4
#include "frameProvider.h"
#endif

#ifdef CHANGED
#ifdef countNew
#include "countNew.h"


	int AddrSpace::nbNewAddrspace = 0;
	int AddrSpace::nbAddrspaceCreated = 0;

	int AddrSpace::getNbNewAddrspace() {
		return nbNewAddrspace;
	}

	int AddrSpace::getNbAddrspaceCreated() {
		return nbAddrspaceCreated;
	}

#endif
#endif

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



#ifdef step4

/**
 * 	STEP 4 constructor
 * 	We don't allocate virtual memory here
 * 	This is done in loadInitialSections which have to be called after
 * 	This permits to test allocation errors
 */
AddrSpace::AddrSpace ()
{
	pageTable = NULL;
	s_exit = new Semaphore("exit semaphore", 0);
	s_stackList = new Semaphore("stack list semaphore", 1);

#ifdef countNew
	nbNewAddrspace++;
	nbAddrspaceCreated++;
	displayNew(nbNewAddrspace, "AddrSpace");
#endif
}

/**
 *  allocate memory needed for the code, data and stack sections
 */
bool AddrSpace::loadInitialSections(OpenFile * executable)
{
	NoffHeader noffH;
	unsigned int size;
	unsigned int i;
	unsigned int availableStackSize;
	bool success;

	// executable header reading
	executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
	if ((noffH.noffMagic != NOFFMAGIC) &&
			(WordToHost (noffH.noffMagic) == NOFFMAGIC))
		SwapHeader (&noffH);
	ASSERT (noffH.noffMagic == NOFFMAGIC);

	// the available stack space begin after the main thread stack
	beginThreadsStackSpace = noffH.code.virtualAddr + noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
	// we align the begin on a page
	beginThreadsStackSpace = (divRoundUp(beginThreadsStackSpace, PageSize) + 1) * PageSize;
	// the size of the virtual memory is the same as the physical memory
	size = MemorySize;
	// handle the memory in terms of pages, to have an aligned size
	numPages = divRoundUp (size, PageSize);
	// virtual memory available for threads stacks
	availableStackSize = (size - beginThreadsStackSpace);
	// the main thread is not included in this number
	maxThreads = availableStackSize / UserStackSize;

	// the stacks space ends with the virtual memory
	endThreadsStackSpace = MemorySize - 1;
	// allocate an allocator for virtual memory
	addrSpaceAllocator = new AddrSpaceAllocator(this, beginThreadsStackSpace, availableStackSize);
	if(addrSpaceAllocator == NULL)
	{
		return false;
	}
	// handle the number of pages needed for a stack
	nbPagesUserStack = divRoundUp(UserStackSize, PageSize);

	DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
			numPages, size);

	pageTable = new TranslationEntry[numPages];
	if(pageTable == NULL)
	{
		delete addrSpaceAllocator;
		return false;
	}
	for(i = 0 ; i < numPages ; i++)
	{
		pageTable[i].valid = FALSE;
		pageTable[i].virtualPage = i;
		pageTable[i].use = FALSE;
		pageTable[i].dirty = FALSE;
		pageTable[i].readOnly = FALSE;
		// init the physical page, but if valid is false this isn't used
		pageTable[i].physicalPage = 0;
	}
	/*DEBUG(',', "CODE MAPPING\n");
	// map the code
	success = mapMem(noffH.code.virtualAddr, noffH.code.size, true);
	if(success)
	{
		DEBUG(',', "INITDATA MAPPING\n");
		// map init data
		success = mapMem(noffH.initData.virtualAddr, noffH.initData.size, true);
	}
	if(success)
	{
		DEBUG(',', "UNINITDATA MAPPING\n");
		// map uninit data
		success = mapMem(noffH.uninitData.virtualAddr, noffH.uninitData.size, true);
	}*/
	DEBUG(',', "EXECUTABLE MAPPING\n");
	success =  mapExecutable(noffH, executable);
	if(success)
	{
		DEBUG(',', "MAIN STACK MAPPING\n");
		// map main thread stack
		success = mapMem(beginThreadsStackSpace - UserStackSize, UserStackSize, true);
	}
	if(!success)
	{
		delete addrSpaceAllocator;
		delete pageTable;
		return false;
	}

	// then, copy in the code and data segments into memory
	if (noffH.code.size > 0)
	{
		DEBUG (',', "Initializing code segment, at %i, size %d\n",
				noffH.code.virtualAddr, noffH.code.size);
		ReadAtVirtual (executable, noffH.code.virtualAddr,
				noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
	}
	if (noffH.initData.size > 0)
	{
		DEBUG (',', "Initializing data segment, at %i, size %d\n",
				noffH.initData.virtualAddr, noffH.initData.size);
		ReadAtVirtual(executable, noffH.initData.virtualAddr,
				noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
	}
	/*if(!mapMem(noffH.code.virtualAddr, noffH.code.size, false))
	{
		return false;
	}
	else
	{
		return true;
	}*/
	return true;
}


bool AddrSpace::mapExecutable(NoffHeader noffH, OpenFile * executable)
{
	int length = noffH.code.size + noffH.initData.size + noffH.uninitData.size;
	return mapMem(noffH.code.virtualAddr, length, true);
}

#else

/**
 *  Constructor for all the steps except step 4
 * 	Allocate all the needed structures
 */
AddrSpace::AddrSpace (OpenFile * executable)
{
	NoffHeader noffH;
	unsigned int size;
	unsigned int i;
#ifdef step3
	unsigned int availableStackSize;
#endif // step3

#ifdef CHANGED
	s_exit = new Semaphore("exit semaphore", 0);
	s_stackList = new Semaphore("stack list semaphore", 1);
#endif // CHANGED

	// executable header reading
	executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
	if ((noffH.noffMagic != NOFFMAGIC) &&
			(WordToHost (noffH.noffMagic) == NOFFMAGIC))
		SwapHeader (&noffH);
	ASSERT (noffH.noffMagic == NOFFMAGIC);

	// how big is address space?

#ifdef step3

//
//		For step3 we use all the memory for the threads stacks
//		They are placed after the main thread stack
//		step4
//

	DEBUG ('a', "Executable informations :\n");
	DEBUG('a', "code size : %d\n", noffH.code.size);
	DEBUG('a', "init data size : : %d\n", noffH.initData.size);
	DEBUG('a', "uninit data size : : %d\n", noffH.uninitData.size);
	// we use all the memory available for the process
	size = MemorySize;
	// the available stack space begin after the main thread stack
	beginThreadsStackSpace = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
	// handle the memory in terms of pages, to have an aligned size
	numPages = divRoundUp (size, PageSize);
	size = numPages * PageSize;
	// handle the space available for threads stacks
	availableStackSize = size - beginThreadsStackSpace;
	// handle the maximum of threads which can beeing running at the same time
	// this number depends on how many stacks can be allocated
	// the main thread is not included in this number
	maxThreads = availableStackSize / UserStackSize;
	// the stacks space ends with the memory
	endThreadsStackSpace = MemorySize - 1;
	// init the available stack pointers for the threads
	initAvailableStackPointers();

#else
	size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
	// to leave room for the stack
	numPages = divRoundUp (size, PageSize);
	size = numPages * PageSize;
#endif // ifdef step3

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
		pageTable[i].physicalPage = i;
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
#ifdef countNew
	nbNewAddrspace++;
	nbAddrspaceCreated++;
	displayNew(nbNewAddrspace, "AddrSpace");
#endif
}

#endif // step4

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Deallocate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
#ifdef step4
	unsigned int i;
	Printf("Destructeur AddrSpace\n");

	DEBUG(',', "FINAL RELEASE\n");
	//release physical pages
	for (i = 0; i < numPages; i++)
	{
		// verify page validity
		if (pageTable[i].valid) {
			frameProvider->ReleaseFrame(pageTable[i].physicalPage);
		}
	}
	Printf("Destructeur AddrSpace : apres releaseFrame\n");
#endif

	// LB: Missing [] for delete
	// delete pageTable;
	delete [] pageTable;
	// End of modification
#ifdef CHANGED
	delete s_exit;
	delete s_stackList;

#ifdef countNew
	nbNewAddrspace--;
	displayNew(nbNewAddrspace, "AddrSpace");
#endif

#ifdef step4
	//TODO: bug a résoudre avant de decommenter
	//delete addrSpaceAllocator;
#endif

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

#ifdef step4// Initial program counter -- must be location of "Start"
	machine->WriteRegister (PCReg, PageSize);

	// Need to also tell MIPS where next instruction is, because
	// of branch delay possibility
	machine->WriteRegister (NextPCReg, PageSize + 4);
#else
	// Initial program counter -- must be location of "Start"
	machine->WriteRegister (PCReg, 0);

	// Need to also tell MIPS where next instruction is, because
	// of branch delay possibility
	machine->WriteRegister (NextPCReg, 4);
#endif

#ifdef step3
	// set the stack register of the main thread after the code and data
	machine->WriteRegister (StackReg, beginThreadsStackSpace - 4);
	DEBUG ('a', "Initializing stack register to %d\n",
			beginThreadsStackSpace);
#else
	// Set the stack register to the end of the address space, where we
	// allocated the stack; but subtract off a bit, to make sure we don't
	// accidentally reference off the end!
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
	// load the page table into the processor
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;
}

#ifdef CHANGED

/**
 * 	returns an initial stack pointer available for a new thread and removes it
 * 	or returns -1 if it's impossible to add a new stack in the address space
 * 	Used for step3
 */
int AddrSpace::popAvailableStackPointer()
{
	int return_value;
	s_stackList->P();
	if(l_availableStackAddress.size() == 0)
	{
		// no more stack address available
		return_value = -1;
	}
	else
	{
		// get a stack address
		return_value = l_availableStackAddress.front();
		l_availableStackAddress.pop_front();
	}
	s_stackList->V();
	return return_value;
}

/**
 * 	add a stackAddr to the list of available stack address
 * 	this stack address must be in the address space
 * 	Used for step3
 */
void AddrSpace::addAvailableStackAddress(unsigned int stackAddr)
{
	ASSERT(stackAddr < (numPages*PageSize) && stackAddr >= beginThreadsStackSpace);
	s_stackList->P();
	l_availableStackAddress.push_back(stackAddr);
	s_stackList->V();
}

/**
 * 	init the stack list with the initial stack pointers
 * 	for the additionnal thread's stacks
 */
void AddrSpace::initAvailableStackPointers()
{
	// stack addresses begin at the page after the main thread stack
	int addr = divRoundUp(beginThreadsStackSpace + UserStackSize, PageSize) * PageSize;
	ASSERT((addr % PageSize) == 0);
	s_stackList->P();
	// add stack addresses in the list
	for(int i = 0 ; i < maxThreads ; i++)
	{
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
	// copie de la table de pages du processeur pour la retablir apres
	TranslationEntry* oldTr = machine->pageTable;
	unsigned int ptSize = machine->pageTableSize;
	// lecture dans l'executable a la position donnee puis le stocke dans le buffer
	nbRead = executable->ReadAt(buffer, numBytes, position);
	// charge la table des pages du processeur
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;
	// copie du buffer en memoire a l’aide de WriteMem
	for (i = 0 ; i < nbRead ; i++)
	{
		machine->WriteMem(virtualaddr+i,1,buffer[i]);
	}
	// retablit la table des pages du processeur
	machine->pageTable = oldTr;
	machine->pageTableSize = ptSize;

	//delete buffer;
}

bool AddrSpace::mapMem(int virtualAddr, int length, bool write)
{
	unsigned int i;
	int frame;
	// number of pages needed for the given length
	unsigned int nbPages = divRoundUp(length, PageSize);
	// index of the begining page of virtualAddr
	unsigned int beginPage = divRoundDown(virtualAddr, PageSize);
	// check integrity
	ASSERT(beginPage + nbPages <= numPages);
	// < 0 = no allocation
	if(length <= 0)
		return true;
	i = 0;
	// get physical frames needed
	do
	{

		// verify if page is already allocated
		if(!pageTable[beginPage + i].valid)
		{
			frame = frameProvider->GetEmptyFrame();
			if(frame != -1)
			{
				// add the gotten page to the virtual memory
				pageTable[beginPage + i].physicalPage = frame;
				pageTable[beginPage + i].valid = TRUE;
				pageTable[beginPage + i].use = FALSE;
				pageTable[beginPage + i].dirty = FALSE;
				pageTable[beginPage + i].readOnly = (int)(!write);
				DEBUG(',', "mapping de la page %i reussi a la frame %i\n", beginPage + i, frame);
			}
		}
		else
		{
			DEBUG(',', "page %i deja allouee\n", beginPage + i);
		}
		i++;
	}
	while(i != nbPages && frame != -1);
	if(i != nbPages)
	{
		// error, no available physical frames
		DEBUG(',', "RELEASE AFTER ERROR\n");
		// free the physical pages we've got
		for(unsigned int j = 0 ; j < i-1 ; j++)
		{
			frameProvider->ReleaseFrame(pageTable[beginPage + j].physicalPage);
			pageTable[beginPage + j].valid = FALSE;
		}
		return false;
	}
	else
	{
		return true;
	}
}


bool AddrSpace::unMapMem(unsigned int beginPage, unsigned int nbPages)
{
	unsigned int i;
	bool allAllocated = true;
	// check the limits
	ASSERT(beginPage + nbPages <= numPages);
	for(i = 0 ; i < nbPages ; i++)
	{
		// release physical frame of the page
		allAllocated &= frameProvider->ReleaseFrame(pageTable[beginPage + i].physicalPage);
		// update validity of the page
		pageTable[beginPage + i].valid = FALSE;
	}
	return allAllocated;
}


bool AddrSpace::setAccessRight(unsigned int beginPage, unsigned int nbPages, bool readOnly)
{
	unsigned int i;
	bool allAllocated = true;
	ASSERT(beginPage + nbPages <= numPages);
	for(i = 0 ; i < nbPages ; i++)
	{
		// check allocation of page
		allAllocated &= pageTable[beginPage + i].valid;
		// change access right
		pageTable[beginPage + i].readOnly = readOnly;
	}
	return allAllocated;
}

int AddrSpace::allocThreadStack()
{
	int return_value;
	// allocate a stack with the virtual memory allocator
	return_value = (addrSpaceAllocator->allocateFirst(UserStackSize+PageSize, true, true));
	if(return_value == -1)
		return -1;
	else
		return return_value + UserStackSize -16;
	//return_value => debut bloc
}

void AddrSpace::freeThreadStack(unsigned int stackAddr)
{
	// DONNER DEBUT DE LA PAGE INTERDITE
	addrSpaceAllocator->free(stackAddr - UserStackSize + 16 - PageSize);
}

void AddrSpace::printMapping(unsigned int max)
{
	unsigned int i;
	for(i = 0 ; i < max && i < numPages ; i++)
	{
#ifdef CHANGED
		Printf("virtual : %i physique : %i\n", i, pageTable[i].physicalPage);
#endif //CHANGED
	}
}

int AddrSpace::mmap(int length)
{
	int addr = addrSpaceAllocator->allocateFirst(length, true, false);
	if(addr == -1)
		return 0;
	else
		return addr;
}

int AddrSpace::unmap(int addr)
{
	return addrSpaceAllocator->free(addr);
}

#endif // step4

#endif

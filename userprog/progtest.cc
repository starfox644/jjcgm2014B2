// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.


#include "copyright.h"
#include "system.h"
#include "console.h"

#include "addrspace.h"
#include "synch.h"

#ifdef step4
#include "synchconsole.h"
//class AddrSpaceAllocator;
#include "addrSpaceAllocator.h"
// On enleve StartProcess de progtest.cc pour le mettre dans process.cc
#endif


#ifndef CHANGED
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
int
StartProcess (char *filename)
{
	OpenFile *executable = fileSystem->Open (filename);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		Exit(-1);
	}
	space = new AddrSpace (executable);
	currentThread->space = space;

	delete executable;		// close file

	space->InitRegisters ();	// set the initial register values
	space->RestoreState ();	// load page table register

	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
	return -1;
}
#endif // changed
// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvail (int arg)
{
	readAvail->V ();
}
static void
WriteDone (int arg)
{
	writeDone->V ();
}

//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void
ConsoleTest (char *in, char *out)
{
	char ch;

	console = new Console (in, out, ReadAvail, WriteDone, 0);
	readAvail = new Semaphore ("read avail", 0);
	writeDone = new Semaphore ("write done", 0);

	for (;;)
	{
		readAvail->P ();	// wait for character to arrive
		ch = console->GetChar ();
#ifdef CHANGED
		if (ch == -1)		// end of file
			return;		// if q, quit
		else if(ch == 'c')	// if c, then write <c>
		{
			console->PutChar ('<');	// echo it!
			writeDone->P ();	// wait for write to finish
			console->PutChar ('c');	// echo it!
			writeDone->P ();	// wait for write to finish
			console->PutChar ('>');	// echo it!
			writeDone->P ();	// wait for write to finish
		}
		else
		{
#endif
			console->PutChar (ch);	// echo it!
			writeDone->P ();	// wait for write to finish
#ifdef CHANGED
		}
#endif
	}
}

//----------------------------------------------------------------------
// SynchConsoleTest
//
//----------------------------------------------------------------------

#ifdef CHANGED
void
SynchConsoleTest (char *in, char *out)
{
	char ch;
	SynchConsole *synchconsoleTest = new SynchConsole(in, out);
	while ((ch = synchconsoleTest->SynchGetChar()) != EOF)
	{
		if (ch == 'c')
		{
			synchconsoleTest->SynchPutChar('<');
			synchconsoleTest->SynchPutChar(ch);
			synchconsoleTest->SynchPutChar('>');
		}
		else
			synchconsoleTest->SynchPutChar(ch);
	}
	fprintf(stderr, "Solaris: EOF detected in SynchConsole!\n");
}
#endif //CHANGED

#ifdef step4
int allocatorTest(char *filename)
{
	//printf("allocator test\n");
	OpenFile *executable = fileSystem->Open (filename);
	AddrSpace *space;
	AddrSpaceAllocator *addrSpaceAlloc;
	int addr;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		return -1;
	}
	space = new AddrSpace ();
	delete executable;		// close file

	// rajouter ici les tests d'allocation
	printf("tests d'allocation\n");
	addrSpaceAlloc = new AddrSpaceAllocator(space, 0, MemorySize);

	addrSpaceAlloc->printFreeList();
	addrSpaceAlloc->printBusyList();
	if ((addr = addrSpaceAlloc->allocateFirst(52, false,true)) == -1)
	{
		printf("***ECHEC ALLOCATION***\n");
	}
	else
	{
		printf("***ALLOCATION A L'ADRESSE %d  OK***\n", addr);
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}

	if ((addr = addrSpaceAlloc->allocateFirst(200, false,false)) == -1)
	{
		printf("***ECHEC ALLOCATION***\n");
	}
	else
	{
		printf("***ALLOCATION A L'ADRESSE %d   OK***\n", addr);
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}

	if (addrSpaceAlloc->free(128) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK en 0***\n");
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}
	printf("LIBERATION DE ADDRSPACEALLOCATOR\n");
	delete addrSpaceAlloc;
	//addrSpaceAlloc->printFreeList();
	//addrSpaceAlloc->printBusyList();
/*
	if ((addr = addrSpaceAlloc->allocateFirst(178, false,false)) == -1)
	{
		printf("***ECHEC ALLOCATION***\n");
	}
	else
	{
		printf("***ALLOCATION A L'ADRESSE %d   OK***\n", addr);
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}

	if ((addr = addrSpaceAlloc->allocateFirst(65139, false,false)) == -1)
	{
		printf("***ECHEC ALLOCATION***\n");
	}
	else
	{
		printf("***ALLOCATION de 65138 A L'ADRESSE %d   OK***\n", addr);
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}

	if (addrSpaceAlloc->free(378) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK en 378***\n");
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}

	if (addrSpaceAlloc->free(178) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK en 178***\n");
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}


	if (addrSpaceAlloc->free(0) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK en 0***\n");
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}



	if (addrSpaceAlloc->free(398) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK en 398***\n");
		addrSpaceAlloc->printFreeList();
		addrSpaceAlloc->printBusyList();
	}*/

/*
	if (addrSpaceAlloc->free(178) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK 178***\n");
	}
	addrSpaceAlloc->printFreeList();
	addrSpaceAlloc->printBusyList();

	if (addrSpaceAlloc->free(378) == -1)
	{
		printf("***ECHEC LIBERATION***\n");
	}
	else
	{
		printf("***LIBERATION OK 378***\n");
	}
	addrSpaceAlloc->printFreeList();
	addrSpaceAlloc->printBusyList();*/
/*
	if ((addr = addrSpaceAlloc->allocateFirst(179, false,true)) == -1)
	{
		printf("***ECHEC ALLOCATION***\n");
	}
	else
	{
		printf("***ALLOCATION A L'ADRESSE %d***\n", addr);
	}
	addrSpaceAlloc->printFreeList();
	addrSpaceAlloc->printBusyList();*/
	printf("Fin allocatorTest\n");
	return 0;
}
#endif

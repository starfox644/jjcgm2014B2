#ifdef CHANGED
#include "system.h"
#include "syscall.h"
#include "process.h"
#include "thread.h"
#include "machine.h"
#include "addrspace.h"
#include <string>

/**
 * Cree un thread et y lance le programme donne en parametre.
 * Renvoie 0 si le thread est bien cree, -1 sinon
 */
int do_forkExec(int adrExec) {
//	char *executable;
////	machine->ReadMem(adrExec, MAX_STRING_SIZE, &executable);
//	Thread *t = new Thread("ThreadForkExec");
//	if (t != NULL)
//	{
//		t->Fork(StartProcess, (char*)executable);
		return 0;
////	}
////	else
//		return -1;
}

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
	currentThread->space->addProcess();
	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
	return -1;

}

#endif

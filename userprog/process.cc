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
	printf("[ForkExec] Debut fonction\n");
	char executable[MAX_STRING_SIZE];
	int c;
	int i = 0;
	// On recupere le chemin du programme a lancer char par char
	machine->ReadMem(adrExec, 1, &c);
	while (i < MAX_STRING_SIZE && c != '\0')
	{
//		printf("%c", c);
		executable[i] = (char) c;
		i++;
		machine->ReadMem(adrExec+i, 1, &c);
	}
//	printf("\n");

	// Si le chemin est plus long que MAX_STRING_SIZE, erreur car il n'y aura pas
	// de '\0' donc le noyau plante
	if (i == MAX_STRING_SIZE)
		return -1;
	else
		executable[i] = '\0';

	Thread *t = new Thread("ThreadForkExec");
	// Si le thread a ete cree et que l'allocation de son espace d'adressage a reussi
	if (t != NULL && allocateProcessSpace(t, executable) != -1)
	{
		t->Fork(UserStartProcess, 0);
		return 0;
	}
	else
		return -1;
}

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * Renvoie -1 en cas d'erreur, 0 sinon
 */
int allocateProcessSpace (Thread *t, char *filename)
{
	printf("[allocateProcessSpace] Debut fonction\n");
	OpenFile *executable = fileSystem->Open (filename);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		return -1;
	}
	/* TODO : Jerem doit changer l'allocation de l'espace d'adressage et je dois changer
	 * cette ligne pour que la fonction puisse retourner une erreur si ca a foire
	 * */
	space = new AddrSpace (executable);
	t->space = space;

	delete executable;		// close file
	return 0;
}

/**
 * Lance le processus dans la machine
 */
void UserStartProcess (int adr)
{
	printf("[UserStartProcess] Debut fonction\n");
	currentThread->space->addProcess(); // ajoute 1 au nb de processus en cours
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();	// load page table register
	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
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
	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
	return -1;

}

#endif

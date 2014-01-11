#ifdef CHANGED
#include "system.h"
//#include "syscall.h"
#include "process.h"
#include "thread.h"
#include "machine.h"
//#include "addrspace.h"
#include <string>
#include "processManager.h"

/**
 * Cree un thread et y lance le programme donne en parametre.
 * Renvoie 0 si le thread est bien cree, -1 sinon
 */
int do_forkExec(int adrExec) {
	//printf("[ForkExec] Debut fonction\n");
	char executable[MAX_STRING_SIZE];
	int c;
	int i = 0;
	s_process->P();
	// On recupere le chemin du programme a lancer char par char
	machine->ReadMem(adrExec, 1, &c);
	while (i < MAX_STRING_SIZE && c != '\0')
	{
		executable[i] = (char) c;
		i++;
		machine->ReadMem(adrExec+i, 1, &c);
	}

	// Si le chemin est plus long que MAX_STRING_SIZE, erreur car il n'y aura pas
	// de '\0' donc le noyau plante
	if (i == MAX_STRING_SIZE)
		return -1;
	else
		executable[i] = '\0';

	//printf("[ForkExec] executable : %s\n", executable);

	Thread *t = new Thread("ThreadForkExec");
	// Si le thread a ete cree et que l'allocation de son espace d'adressage a reussi
	if (t != NULL && allocateProcessSpace(t, executable) != -1)
	{
		addProcess(); // ajoute 1 au nb de processus en cours
		//printf("[ForkExec] allocate reussi\n");
		t->Fork(UserStartProcess, 0);
		s_process->V();
		return 0;
	}
	else
	{
		s_process->V();
		return -1;
	}
}

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * Renvoie -1 en cas d'erreur, 0 sinon
 */
int allocateProcessSpace (Thread *t, char *filename)
{
	bool isSuccess;
	//printf("[allocateProcessSpace] Debut fonction\n");
	OpenFile *executable = fileSystem->Open (filename);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		return -1;
	}
	//space = new AddrSpace (executable);
	//printf("appel a addrSpace avec %s\n", t->getName());
	space = new AddrSpace();
	isSuccess = space->loadInitialSections(executable);
	if (!isSuccess)
		return -1;
	t->space = space;

	delete executable;		// close file
	return 0;
}

/**
 * Lance le processus dans la machine
 */
void UserStartProcess (int adr)
{
	//printf("[UserStartProcess] Creation du processus pour progSimple\n");
	//printf("[UserStartProcess] Debut fonction\n");
//	printf("[UserStartProcess] addProcess\n");
	currentThread->space->setPid(nbProcess);
	currentThread->space->processRunning = true;
	processManager->addAddrProcess(currentThread->space);
	currentThread->space->InitRegisters ();	// set the initial register values
//	printf("[UserStartProcess] InitRegisters\n");
	currentThread->space->RestoreState ();	// load page table register
	//printf("[UserStartProcess] lancement de %s pid : %i\n", currentThread->getName(), currentThread->space->getPid());
	fflush(stdout);
//	printf("[UserStartProcess] RestoreState\n");
	//printf("[UserStartProcess] nbProc : %i\n", currentThread->space->getNbProcess());
	machine->Run ();		// jump to the user program
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
}

void addProcess ()
{
	s_nbProcess->P();
	currentThread->space->setPid(nbProcess);
	nbProcess++;
	s_nbProcess->V();
}

void removeProcess () {
	s_nbProcess->P();
	nbProcess--;
	s_nbProcess->V();
}

int getNbProcess () {
	return nbProcess;
}
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
int
StartProcess (char *filename)
{
	//printf("[StartProcess] Creation du processus pour forkexecsimple\n");
	OpenFile *executable = fileSystem->Open (filename);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		Exit(-1);
	}
#ifndef step4
	space = new AddrSpace (executable);
#else
	space = new AddrSpace();
	if(!space->loadInitialSections(executable))
	{
		delete executable;		// close file
		delete space;
		return -1;
	}
#endif
	currentThread->space = space;

	delete executable;		// close file
	addProcess(); // ajoute 1 au nb de processus en cours
	space->InitRegisters ();	// set the initial register values
	space->RestoreState ();	// load page table register
	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
	return -1;

}

#endif

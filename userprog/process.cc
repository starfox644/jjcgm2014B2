#ifdef CHANGED
#include "machine.h"
#include "syscall.h"
#include "system.h"
#include "threadManager.h"

#ifdef step4
/**
 * Cree un thread et y lance le programme donne en parametre.
 * Renvoie 0 si le thread est bien cree, -1 sinon
 */
int do_forkExec(int adrExec)
{
	printf("[ForkExec] Debut fonction\n");
	char executable[MAX_STRING_SIZE];
	int c;
	int i = 0;
	s_createProcess->P();
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
		s_createProcess->V();
		return 0;
	}
	else
	{
		s_createProcess->V();
		return -1;
	}
}

/**
 * Lance le processus dans la machine
 */
void UserStartProcess (int adr)
{
	AddrSpace *space = currentProcess->getAddrSpace();
	currentProcess->setPid(nbProcess);
	currentProcess->processRunning = true;
	processManager->addAddrProcess(space);
	space->InitRegisters ();	// set the initial register values
	space->RestoreState ();	// load page table register
	machine->Run ();		// jump to the user program
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
}

void addProcess ()
{
	s_nbProcess->P();
	currentProcess->setPid(nbProcess);
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

#endif

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * Renvoie -1 en cas d'erreur, 0 sinon
 */
int allocateProcessSpace (Thread *t, char *filename)
{
	//printf("[allocateProcessSpace] Debut fonction\n");
	OpenFile *executable = fileSystem->Open (filename);

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		return -1;
	}
	Process* process = NULL;
	process = new Process();
	if(process == NULL || !process->allocateAddrSpace(executable))
	{
		delete executable;		// close file
		return -1;
	}
	t->process = process;
	delete executable;		// close file
	return 0;
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
	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		Exit(-1);
	}

	Process* process = new Process();
	if(!process->allocateAddrSpace(executable))
	{
		delete executable;		// close file
		return -1;
	}
	delete executable;		// close file
	currentThread->process = process;
	currentProcess = process;
#ifdef step4
	addProcess(); // ajoute 1 au nb de processus en cours
#endif
	process->getAddrSpace()->InitRegisters ();	// set the initial register values
	process->getAddrSpace()->RestoreState ();	// load page table register
	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
	return -1;
}

Process::Process()
{
	addrSpace = NULL;
	processRunning = false;
	threadManager = new ThreadManager();
}

bool Process::allocateAddrSpace(OpenFile * executable)
{
	bool return_value = true;
#ifdef step4
	addrSpace = new AddrSpace();
	if(addrSpace != NULL)
	{
		// load code and initial data
		return_value = addrSpace->loadInitialSections(executable);
	}
	else
	{
		return_value = false;
	}
#else
	addrSpace = new AddrSpace(executable);
#endif
	return return_value;
}

void Process::freeAddrSpace()
{
	delete addrSpace;
	delete threadManager;
	addrSpace = NULL;
}

AddrSpace* Process::getAddrSpace()
{
	return addrSpace;
}


int Process::getPid()
{
	return pid;
}

void Process::setPid(int newPid)
{
	pid = newPid;
}

#endif

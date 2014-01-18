#ifdef CHANGED
#include "machine.h"
#include "syscall.h"
#include "system.h"
#include "semaphoreManager.h"
#include "threadManager.h"
#include <list>

extern void do_exit(int returnCode);
class Thread;

#ifdef step4

/**
 * Realise l'appel system fork exec.
 * Lit le nom de l'executable dans la memoire du MIPS a partir de l'adresse passee en
 * parametre et cree un thread principal pour le processus.
 * Appelle ensuite allocateProcessSpace avec ces infos.
 * Renvoie 0 si le processus a pu etre cree, -1 sinon
 */
int do_forkExec(int adrExec)
{
	char executable[MAX_STRING_SIZE];
	int c;
	int i = 0;

	// cree une section critique pour creer un processus
	s_createProcess->P();
	// On recupere le chemin du programme a lancer char par char
	machine->ReadMem(adrExec, 1, &c);
	while (i < MAX_STRING_SIZE && c != '\0')
	{
		executable[i] = (char) c;
		i++;
		machine->ReadMem(adrExec+i, 1, &c);
	}

	// Si le chemin est plus long que MAX_STRING_SIZE, erreur car il n'y aura pas de '\0'
	// sinon on rajoute un '\0'
	if (i == MAX_STRING_SIZE)
		return -1;
	else
		executable[i] = '\0';

	// creation du thread principal du processus
	Thread *t = NULL;
	t = new Thread("ThreadForkExec");
	if(t == NULL)
	{
		printf("[ForkExec] Erreur creation thread\n");
		// relachement de la section critique de creation
		s_createProcess->V();
		return -1;
	}
	else
	{
		// test d'allocation du processus
		if (allocateProcessSpace(t, executable) != -1)
		{
			// ajoute 1 au nb de processus en cours
			addProcess();
			// creation du thread principal
			t->Fork(UserStartProcess, 0);
			// relachement de la section critique de creation
			s_createProcess->V();
			return t->process->getPid();
		}
		else
		{
			//printf("[ForkExec] Erreur allocation process space\n");
			// erreur : l'allocation du processus a echoue
			delete t;
			// relachement de la section critique de creation
			s_createProcess->V();
			return -1;
		}
	}
}

/**
 * 	Initialise l'etat du processus (registres et chargement de la table des pages)
 * 	adr n'est pas utilise, il n'est present que pour le prototype de la fonction
 * 	necessaire au thread.
 */
void UserStartProcess (int adr)
{
	// intialisation du processus en section critique
	s_createProcess->P();
	// recuperation de l'espace d'adressage du processus
	AddrSpace *space = currentProcess->getAddrSpace();
	// indication du lancement du processus
	currentProcess->processRunning = true;
	currentProcess->semProc->P();
	// initialisation de l'etat du processus
	space->InitRegisters ();
	space->RestoreState ();
	// relachement de section critique
	s_createProcess->V();
	// lancement du programme
	machine->Run ();
	// on ne revient jamais ici si tout se passe normalement
	// le processus se termine par un appel a exit
	ASSERT (FALSE);
}

void addProcess ()
{
	s_nbProcess->P();
	nbProcess++;
	s_nbProcess->V();
}

void removeProcess () {
	s_nbProcess->P();
	processManager->removeAddrProcess(currentProcess);
	nbProcess--;
	s_nbProcess->V();
}

int getNbProcess () {
	return nbProcess;
}

#endif // step4

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * Renvoie -1 en cas d'erreur, 0 sinon
 */
int allocateProcessSpace (Thread *t, char *filename)
{
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
#ifdef step4
	int pid = processManager->getNextPid();
	if(pid == -1)
	{
		// erreur : plus de pid disponible
		process->freeAddrSpace();
		delete process;
		delete executable;
		return -1;
	}
	process->setPid(pid);
	processManager->addAddrProcess(process);
#endif
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
	OpenFile *executable = fileSystem->Open (filename);
	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", filename);
		return -1;
	}

	Process* process = NULL;
	process = new Process();
	if(process == NULL)
	{
		delete executable;		// close file
		return -1;
	}
	else if(!process->allocateAddrSpace(executable))
	{
		delete process;
		delete executable;		// close file
		return -1;
	}

	delete executable;		// close file

	// assignement du processus cree au thread actuel
	currentThread->process = process;
	// le processus cree devient le processus actuel (premier processus lance)
	currentProcess = process;
#ifdef step4
	process->setPid(processManager->getNextPid());
	processManager->addAddrProcess(currentProcess);
	currentProcess->processRunning = true;
	currentProcess->semProc->P();
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

#ifdef step4

int do_mmap(int length)
{
	return currentProcess->getAddrSpace()->mmap(length);
}
int do_unmap(int addr)
{
	return currentProcess->getAddrSpace()->unmap(addr);
}

#endif

/**
 * 	Cree un processus vide.
 * 	Pour le chargement d'un programme, allocateAddrSpace doit etre appele.
 */
Process::Process()
{
	addrSpace = NULL;
	processRunning = false;
	mainIsWaiting = false;
	estAttendu = false;
	threadManager = new ThreadManager();
	semManager = new SemaphoreManager();
	semProc = new Semaphore("semaphore processus", 1);

}

Process::~Process() {
	delete semProc;
}

/**
 * 	alloue un espace d'adressage pour le processus en chargeant l'executable a l'interieur
 */
bool Process::allocateAddrSpace(OpenFile * executable)
{
	bool return_value = true;
#ifdef step4
	addrSpace = NULL;
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
	// les etapes precedentes chargent directement le code dans le constructeur
	addrSpace = new AddrSpace(executable);
#endif // step4
	return return_value;
}

/**
 * 	libere l'espace d'adressage du processus
 */
void Process::freeAddrSpace()
{
	delete addrSpace;
	threadManager->deleteThreads();
	delete threadManager;
	delete semManager;
	addrSpace = NULL;
}

/**
 * 	Renvoie un pointeur sur l'espace d'adressage du processus
 */
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

/**
 * Demande aux threads du processus de se terminer
 */
void Process::killProcess()
{
	interrupt->SetLevel (IntOff);
	std::list<Thread*>::iterator it = threadManager->l_threads.begin();
	scheduler->RemoveTid(0);
	while (it != threadManager->l_threads.end())
	{
		scheduler->RemoveTid((*it)->tid);
		++it;
	}
	freeAddrSpace();
	if(scheduler->isReadyListEmpty())
	{
		interrupt->Halt();
	}
	else
	{
		currentThread->Finish();
	}

}
bool Process::getEstAttendu()
{
	return estAttendu;
}

void Process::setEstAttendu(bool b)
{
	estAttendu = b;
}

#endif // CHANGED

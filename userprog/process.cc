#ifdef CHANGED
#include "machine.h"
#include "syscall.h"
#include "system.h"
#include "semaphoreManager.h"
#include "threadManager.h"

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
			return 0;
		}
		else
		{
			// erreur : l'allocation du processus a echoue
			delete t;
			// relachement de la section critique de creation
			s_createProcess->V();
			return -1;
		}
	}
}

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * t : thread principal du nouveau processus
 * filename : nom du programme a executer
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
	if(process == NULL)
	{
		// erreur d'allocation du processus

		delete executable;
		return -1;
	}
	// allocation d'un espace d'adressage
	else if(!process->allocateAddrSpace(executable))
	{
		// erreur d'allocation de l'espace

		delete process;
		delete executable;
		return -1;
	}
	else
	{
		// assignement du processus cree au thread principal

		t->process = process;
		delete executable;
		return 0;
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
	// attribution d'un pid au processus
	currentProcess->setPid(nbProcess);
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

#endif // step4

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
		Exit(-1);
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
	semManager = new SemaphoreManager();
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
	// les etapes precedentes chargent directement le code dans le constructeur
	addrSpace = new AddrSpace(executable);
#endif // step4
	return return_value;
}

void Process::freeAddrSpace()
{
	delete addrSpace;
	delete threadManager;
	delete semManager;
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

#endif // CHANGED

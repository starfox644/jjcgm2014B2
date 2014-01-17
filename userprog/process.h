#ifdef CHANGED
#ifndef ___PROCESS
#define ___PROCESS

#include "addrspace.h"
class Thread;
class OpenFile;
class SemaphoreManager;
class ThreadManager;

void addProcess ();

void removeProcess () ;

int getNbProcess ();

/**
 * Cree un thread et lance le programme donne en parametre dedans.
 * Renvoie 0 si le thread est bien cree, -1 sinon
 */
int do_forkExec(int adrExec);

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * Renvoie -1 en cas d'erreur, 0 sinon
 */
int allocateProcessSpace (Thread *t, char *filename);

/**
 * Lance le processus dans la machine
 */
void UserStartProcess (int adr);

//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
int StartProcess (char *filename);

class Process
{
	private:
		AddrSpace *addrSpace;
	    int pid;
	public:
		SemaphoreManager *semManager;
		ThreadManager *threadManager;
		Process();
		bool allocateAddrSpace(OpenFile * executable);
		void freeAddrSpace();
		AddrSpace* getAddrSpace();
	    void setPid(int newPid);
	    int getPid();

		bool processRunning; //true si en cours d'exécution false sinon
};

#endif // CHANGED
#endif // ___PROCESS

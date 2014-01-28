#ifdef CHANGED
#ifndef ___PROCESS
#define ___PROCESS

#include "addrspace.h"
class Thread;
class OpenFile;
class SemaphoreManager;
class ThreadManager;
#include "filemanager.h"

#define NB_FILES_PROCESS 10

/**
 * Realise l'appel systeme fork exec.
 * Lit le nom de l'executable dans la memoire du MIPS a partir de l'adresse passee en
 * parametre et cree un thread principal pour le processus.
 * Appelle ensuite allocateProcessSpace avec ces infos.
 * Renvoie 0 si le processus a pu etre cree, -1 sinon
 */
int do_forkExec(int adrExec);

/**
 * Alloue l'espace necessaire au processus pour son programme.
 * t : thread principal du nouveau processus
 * filename : nom du programme a executer
 * Renvoie -1 en cas d'erreur, 0 sinon
 */
int allocateProcessSpace (Thread *t, char *filename);

/**
 * 	Initialise l'etat du processus (registres et chargement de la table des pages)
 * 	adr n'est pas utilise, il n'est present que pour le prototype de la fonction
 * 	necessaire au thread.
 */
void UserStartProcess (int adr);

int do_mmap(int length);
int do_unmap(int addr);

//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
int StartProcess (char *filename);

class Process
{
	private:
		// gere la memoire virtuelle du processus
		AddrSpace *addrSpace;
		// identifiant unique du processus
	    int pid;
	    // true si un processus quelconque attend ce processus, false sinon
		bool estAttendu;
		// nombre de fichiers ouverts par le processus
		int nbOpenFiles;
		// id noyaux de fichiers ouverts par le processus
		// l'indice de tableau correspond a l'id relatif au process
		OpenFileId openFiles[NB_FILES_PROCESS];

	public:
	    /**
	     * 	Cree un processus vide.
	     * 	Pour le chargement d'un programme, allocateAddrSpace doit etre appele.
	     */
		Process();
		~Process();

	    // gere les semaphores utilisateurs du processus
		SemaphoreManager *semManager;
		// gere les threads crees du processus
		Semaphore *semProc;
		ThreadManager *threadManager;

		/**
		 * alloue un espace d'adressage pour le processus en chargeant l'executable a l'interieur
		 */
		bool allocateAddrSpace(OpenFile * executable);

		/**
		 * libere l'espace d'adressage du processus
		 */
		void freeAddrSpace();

		/**
		 * 	Renvoie un pointeur sur l'espace d'adressage du processus
		 */
		AddrSpace* getAddrSpace();
	    void setPid(int newPid);
	    int getPid();
		bool getEstAttendu();
		void setEstAttendu(bool b);

	    // vaut true si le processus a ete lance, false s'il s'est termine
		bool processRunning;
		// indique si le thread main attend la terminaison des autres
	    bool threadWaiting;

	    /**
	     * 	Demande aux threads du processus de se terminer
	     */
	    void killProcess();

	    /**
	     * 	Renvoie true si le processus peut ajouter un nouveau fichier ouvert a sa table.
	     */
	    bool canAddFile();

	    /**
	     *	Ajoute un fichier ouvert au processus lorsqu'il appelle open.
	     *	Cette fonction ne doit etre appelee que si canAddFile renvoie true.
	     *	Leve une assertion si le processus n'a plus de place disponible pour un nouveau fichier.
	     */
	    int addOpenFile(OpenFileId id);

	    /**
	     * 	Retire le fichier ouvert id de la table du processus.
	     */
	    bool removeOpenFile(int id);

	    /**
	     * 	Renvoie l'id de fichier du noyau a partir de l'id du processus.
	     */
	    OpenFileId getOpenFileId(int processFileId);
};

#endif // CHANGED
#endif // ___PROCESS

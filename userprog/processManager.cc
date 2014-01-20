#ifdef step4
/*
 * processManager.cc
 * Classe permettant de gérer une liste d'adresse de processus afin de faire une attente d'un processus défini.
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "processManager.h"
#include "system.h"

ProcessManager::ProcessManager ()
{
	//on initialise notre semaphore
	sem_Wait = new Semaphore("WaitPid semaphore", 1);
	nbAdrProcess = 0;
	nextPid = 0;
}

ProcessManager::~ProcessManager()
{
	std::list<Process*>::iterator it;

	for(it = l_process.begin() ; it != l_process.end() ; ++it)
	{
		delete (*it);
	}
}

int ProcessManager::addAddrProcess(Process *proc){
	sem_Wait->P();

	nbAdrProcess++;
	//rajout de notre adresse de processus dans la liste
	l_process.push_back(proc);
	sem_Wait->V();
	return 0;
}
void ProcessManager::removeAddrProcess(Process *proc){
	sem_Wait->P();
	nbAdrProcess--;
	proc->processRunning = false;
	sem_Wait->V();
}
int ProcessManager::getNbAddrProcess(){
	return nbAdrProcess;
}

/*
 * Met le processus courant en attente du processus dont le pid est passe en parametre.
 * Renvoie -1 si le processus a attendre est le processus courant ou si le pid n'est pas dans la liste.
 * Renvoie le pid du processus attendu jusqu'a terminaison sinon.
 *
 */
int ProcessManager::waitPid(int processPid){
	sem_Wait->P();
	// iterator pour trouver l'adresse dans la liste
	std::list<Process*>::iterator it=l_process.begin();

	while (it != l_process.end() && (*it)->getPid() != processPid)
		it++;

	// si le pid du process n'est pas trouve, return -1 : error
	if (it == l_process.end())
	{
		printf("[WaitPid] Erreur - Processus introuvable dans la liste\n");
		sem_Wait->V();
		return -1;
	}
	// si le process a attendre est le process courant : -1
	else if ((*it)->getPid() == currentProcess->getPid())
	{
		printf("[WaitPid] Erreur - Un processus ne peut s'attendre lui-meme\n");
		sem_Wait->V();
		return -1;
	}
	// Si le process est deja attendu TODO : a tester, on ne peut pas pour l'instant
	else if ((*it)->getEstAttendu())
	{
		printf("[WaitPid] Erreur - Ce processus est deja attendu par un autre processus\n");
		sem_Wait->V();
		return -1;
	}
	else
	{
		(*it)->setEstAttendu(true);
		sem_Wait->V();
		(*it)->semProc->P();
		int procPid = (*it)->getPid(); 				// Recup PID pour le renvoyer
		(*it)->semProc->V();
		sem_Wait->P();
		l_process.erase(it); 					// On supprime le processus de la liste
		sem_Wait->V();

		// On libere la ressource
		return procPid;

	}
	return 0;
}

/**
 * Renvoie le prochain PID non utilise, jusqu'a MAX_INT
 */
int ProcessManager::getNextPid()
{
	if(nextPid <= INT_MAX)
	{
		return nextPid++;
	}
	else
	{
		return -1;
	}
}
#endif //step4




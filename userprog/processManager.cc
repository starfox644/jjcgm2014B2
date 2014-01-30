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
	nbProcessRunning = 0;
	nbProcessTotal = 0;
	nextPid = 0;
}

ProcessManager::~ProcessManager()
{
	std::list<Process*>::iterator it;

	for(it = l_process.begin() ; it != l_process.end() ; ++it)
	{
		delete (*it);
	}
	delete sem_Wait;
}

int ProcessManager::addProcess(Process *proc){
	sem_Wait->P();

	nbProcessRunning++;
	nbProcessTotal++;
	//rajout de processus dans la liste
	l_process.push_back(proc);
	sem_Wait->V();
	return 0;
}

void ProcessManager::removeProcess(Process *proc){
	sem_Wait->P();
	nbProcessRunning--;
	proc->processRunning = false;
	sem_Wait->V();
}

int ProcessManager::getNbProcessRunning(){
	return nbProcessRunning;
}

int ProcessManager::getNbProcessTotal(){
	return nbProcessTotal;
}

/*
 * Met le processus courant en attente du processus dont le pid est passe en parametre.
 * Renvoie -1 si le processus a attendre est le processus courant, si le pid n'est pas dans la liste
 * ou si le processus est deja attendu par un autre.
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
		sem_Wait->V();
		return -1;
	}
	// si le process a attendre est le process courant : -1
	else if ((*it)->getPid() == currentProcess->getPid())
	{
		sem_Wait->V();
		return -1;
	}
	// Si le process est deja attendu
	else if ((*it)->getEstAttendu())
	{
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
		(*it)->setEstAttendu(false);

		// On libere la ressource
		return procPid;

	}
	return 0;
}

/**
 * Renvoie le prochain PID non utilise, jusqu'a INT_MAX
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

/*
 * Renvoi une liste des processus
 */
int ProcessManager::getListProcess(int ListeProc){

	int ecart = 0;
	sem_Wait->P();
	// iterator pour trouver l'adresse dans la liste
	std::list<Process*>::iterator it=l_process.begin();

	while (it != l_process.end()){
		machine->WriteMem(ListeProc + ecart,sizeof(int),(*it)->getPid());
		ecart = ecart + sizeof(int);
		it++;
	}
	ecart = ecart + sizeof(int);
	sem_Wait->V();
	return ListeProc;
}
#endif //step4

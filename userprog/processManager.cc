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
	delete sem_Wait;
}

int ProcessManager::addAddrProcess(Process *proc){
	sem_Wait->P();

	/*if(proc->processRunning){ // si le programme est en cours d'execution on le rajoute
		nbAdrProcess++;
		//rajout de notre adresse de processus dans la liste
		l_process.push_back(proc);
	}else{ //le programme n'est pas en cours d'execution donc on a une erreur
		sem_Wait->V();
		return -1;
	}*/

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
 * Fonction en cours de réalisation
 */
int ProcessManager::waitPid(int processPid){
	sem_Wait->P();
	// iterator pour trouver l'adresse dans la liste
	std::list<Process*>::iterator it=l_process.begin();

	while (it != l_process.end() && (*it)->getPid() != processPid)
	{
		it++;
	}
	if(it == l_process.end())
	{
		sem_Wait->V();
		return -1;
	}
	// si l'adresse du process n'est pas trouvee, return -1 : error
	if ((*it)->getPid() != processPid || (*it)->getPid() == currentProcess->getPid()){
		sem_Wait->V();
		return -1;
	}else{

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




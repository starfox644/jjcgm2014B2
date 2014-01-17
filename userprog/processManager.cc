#ifdef step4
/*
 * processManager.cc
 * Classe permettant de gérer une liste d'adresse de processus afin de faire une attente d'un processus défini.
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "processManager.h"

ProcessManager::ProcessManager (){
	//on initialise notre semaphore
	sem_Wait = new Semaphore("WaitPid semaphore", 1);
	nbAdrProcess = 0;
}

int ProcessManager::addAddrProcess(Process *proc){
	sem_Wait->P();
	if(adr->processRunning){ // si le programme est en cours d'execution on le rajoute
		nbAdrProcess++;
		//rajout de notre adresse de processus dans la liste
		l_process.push_back(proc);
	}else{ //le programme n'est pas en cours d'execution donc on a une erreur
		sem_Wait->V();
		return -1;
	}

	sem_Wait->V();*/
	return 0;
}
void ProcessManager::removeAddrProcess(Process *proc){
	sem_Wait->P();
	nbAdrProcess--;
	proc->RunningStatus = false;
	sem_Wait->V();
}
int ProcessManager::getNbAddrProcess(){
	return nbAdrProcess;
}

/*
 * Fonction en cours de réalisation
 * Je suis pas fan de l'attente active que j'ai faite pour le moment. Faut que je change ca >_<
 */
int ProcessManager::WaitPid(int processPid){
	/*sem_Wait->P();
	// iterator pour trouver l'adresse dans la liste
	std::list<Process*>::iterator it=l_process.begin();
	while (it != l_process.end() && (*it)->pid != processPid)
		it++;
	// si l'adresse du process n'est pas trouvee, return -1 : error
	if ((*it)->pid != processPid){
		sem_Wait->V();
		return -1;
	}else{

		while((*it)->processRunning == true){	// On attend que le processus termine
			sem_Wait->V();
			sem_Wait->P();
		}int procPid = (*it)->pid; 				// Recup PID pour le renvoyer
		l_process.erase(it); 					// On supprime le processus de la liste
		sem_Wait->V();							// On libere la ressource
		return procPid;

	}*/
	return 0;
}
#endif //step4




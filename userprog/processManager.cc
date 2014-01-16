/*
 * processManager.cc
 *Classe permettant de gérer une liste d'adresse de processus afin de faire une attente d'un processus défini.
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#ifdef step4
#include "processManager.h"


ProcessManager::ProcessManager (){
	//on initialise notre semaphore
	sem_Wait = new Semaphore("WaitPid semaphore", 1);
	nbAdrProcess = 0;
}

int ProcessManager::addAddrProcess(AddrSpace *adr){
	sem_Wait->P();
	if(adr->processRunning){ // si le programme est en cours d'execution on le rajoute
		nbAdrProcess++;
		//rajout de notre addresse de processus dans la liste
		l_process.push_back(adr);
	}else{ //le programme n'est pas en cours d'execution donc on a une erreur
		sem_Wait->V();
		return -1;
	}

	sem_Wait->V();
	return 0;
}
void ProcessManager::removeAddrProcess(AddrSpace *adr){
	sem_Wait->P();
	nbAdrProcess--;
	sem_Wait->V();
}
int ProcessManager::getNbAddrProcess(){
	return nbAdrProcess;
}

/*
 * Fonction en cours de réalisation
 * Je suis pas fan de l'attente active que j'ai faite pour le moment. Faut que je change ça >_<
 */
int ProcessManager::WaitPid(int processPid){
		sem_Wait->P();
		// iterator pour trouver l'adresse dans la liste
		std::list<AddrSpace*>::iterator it=l_process.begin();
		while (it != l_process.end() && (*it)->pid != processPid)
			it++;
		// si l'adresse du process n'est pas trouve, return -1 : error
		if ((*it)->pid != processPid){
			sem_Wait->V();
			return -1;
		}else{
			do{// temps que notre processuss tourne on attends
				if((*it)->processRunning == false){ // si le processuss est deja arreté alors on le supprime de la list et on renvoi le pid
					int procPid = (*it)->pid;
					l_process.erase(it);
					sem_Wait->V();
					return procPid;
				}
			}while((*it)->processRunning == true);
			sem_Wait->V();
			return 0;

		}
}
#endif //step4




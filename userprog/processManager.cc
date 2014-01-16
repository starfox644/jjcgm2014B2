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

int ProcessManager::addAddrProcess(AddrSpace *adr){
	/*sem_Wait->P();
	if(adr->processRunning){ // si le programme est en cours d'execution on le rajoute
		nbAdrProcess++;
		//rajout de notre adresse de processus dans la liste
		l_process.push_back(adr);
	}else{ //le programme n'est pas en cours d'execution donc on a une erreur
		sem_Wait->V();
		return -1;
	}

	sem_Wait->V();*/
	return 0;
}
void ProcessManager::removeAddrProcess(AddrSpace *adr){
	/*sem_Wait->P();
	nbAdrProcess--;
	sem_Wait->V();*/
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
	std::list<AddrSpace*>::iterator it=l_process.begin();
	while (it != l_process.end() && (*it)->pid != processPid)
		it++;
	// si l'adresse du process n'est pas trouvee, return -1 : error
	if ((*it)->pid != processPid){
		sem_Wait->V();
		return -1;
	}else{*/
		/* Version Mika : ton attente active peut foirer puisque si processRunning passe a false
		 * a la fin de ton if, il retourne pas dans ta boucle donc la fonction return 0 et le processus
		 * est toujours dans la liste.
		do{// tant que notre processus tourne on attend
			if((*it)->processRunning == false){ // si le processus est deja arrete alors on le supprime de la liste et on renvoie le pid
				int procPid = (*it)->pid;
				l_process.erase(it);
				sem_Wait->V();
				return procPid;
			}
		}while((*it)->processRunning == true);
		sem_Wait->V();
		return 0;
		*/

		/* Version corrigee (je suppose que c'est bon) */
	/*
		while((*it)->processRunning == true);	// On attend que le processus termine
		int procPid = (*it)->pid; 				// Recup PID pour le renvoyer
		l_process.erase(it); 					// On supprime le processus de la liste
		sem_Wait->V();							// On libere la ressource
		return procPid;

	}*/
	return 0;
}
#endif //step4




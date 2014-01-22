/*
* processManager.h
*
*  Created on: 10 janv. 2014
*      Author: galdween
*/
#ifdef step4
#ifndef __PROCESSMANAGER__H__
#define __PROCESSMANAGER__H__

#include "synch.h"

class ProcessManager{

	public :
		//constructeur
		//creation d'un processManager
		ProcessManager ();
		~ProcessManager();
		//variables
		Semaphore *sem_Wait;	//semaphore pour notre classe
		// liste des addrSpace de chaque processus
		std::list<Process*> l_process;

		//rajout d'un processus a notre liste
		int addProcess(Process *);
		//suppression d'un processus de notre liste
		void removeProcess(Process *);
		//récupération du nombre de processus
		int getNbProcessRunning();
		//récupération du nombre de processus total
		int getNbProcessTotal();

		int getNextPid();

		//fonctions
		/* permet d'attendre le pid voulu avant de continuer
		*	retourne -1 en cas d'erreur sinon retourne 0
		**/
		int waitPid(int processPid);

		/*
		 * fonction permettant de retourner la list des processus
		 */
		int getListProcess(int);

	private:
		int nextPid;			// numero du prochain processus a creer
		int nbProcessRunning;	// nombre de processus de notre liste
		int nbProcessTotal;		// nombre total de processus de notre liste

};
#endif // __PROCESSMANAGER__H__
#endif //step4

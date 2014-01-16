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

		//variables
		int nbAdrProcess;	// nombre d'adresse de processus de notre liste
		Semaphore *sem_Wait;	//semaphore pour notre classe
		// liste des addrSpace de chaque processus
		std::list<AddrSpace*> l_process;

		//rajout d'une addresse d'un processus a notre liste
		int addAddrProcess(AddrSpace *);
		//suppression d'une addresse d'un processus de notre liste
		void removeAddrProcess(AddrSpace *);
		//récupération du nombre d'adresse de processus
		int getNbAddrProcess();

		//fonctions
		/* permet d'attendre le pid voulu avant de continuer
		*	retourne -1 en cas d'erreur sinon retourne 0
		**/
		int WaitPid(int );

};
#endif // __PROCESSMANAGER__H__
#endif //step4





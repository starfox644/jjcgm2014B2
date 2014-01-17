#ifdef CHANGED
#include "system.h"
#include "syscall.h"
#include <iostream>

#include "semaphoreManager.h"
#ifdef countNew
#include "countNew.h"
#endif

/**
*	Demande l'initialisation d'un semaphore noyau
*	Renvoie 0 si le semaphore est cree, -1 sinon
*/
int do_SemInit(int adrSem, int initValue)
{
	int id;
	bool isSuccess;
	id = currentProcess->semManager->addSemaphore(initValue);
	isSuccess = machine->WriteMem(adrSem, 4, id);
	if (isSuccess == 1)
		return 0;
	else
		return -1;
}

/**
 * Demande au semaphore identifie par id d'ajouter le thread courant
 * a la liste des threads en attente pour la ressource
 */
int do_SemWait(int id)
{
	Semaphore *sem;
	sem = currentProcess->semManager->getSemaphore(id);
	if (sem != NULL)
	{
		printf("[SemWait] Mis en attente du thread #%i du proc #%i sur le semaphore #%i\n", currentThread->getTid(), currentProcess->getPid(), id);
		sem->P();
		printf("[SemWait] Attente terminee du thread #%i du proc #%i sur le semaphore #%i\n", currentThread->getTid(), currentProcess->getPid(), id);
		return 0;
	}
	else
		return -1;
}

/**
 * Demande au semaphore identifie par id de retirer le thread courant
 * de la liste des threads en attente pour la ressource
 */
int do_SemPost(int id)
{
	Semaphore *sem;
		sem = currentProcess->semManager->getSemaphore(id);
		if (sem != NULL)
		{
			sem->V();
			return 0;
		}
		else
			return -1;
}

/**
 * Demande la suppression du semaphore identifie par id
 */
int do_SemDestroy(int id)
{
	if (currentProcess->semManager->removeSemaphore(id) != -1)
		return 0;
	else
		return -1;
}

#endif // CHANGED

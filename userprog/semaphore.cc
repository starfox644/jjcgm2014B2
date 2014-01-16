#ifdef CHANGED
#include "system.h"
#include "syscall.h"
#include <iostream>

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
	id = currentThread->process->semManager->addSemaphore(initValue);
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
	sem = currentThread->process->semManager->getSemaphore(id);
	if (sem != NULL)
	{
		sem->P();
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
		sem = currentThread->process->semManager->getSemaphore(id);
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
	if (currentThread->process->semManager->removeSemaphore(id) != -1)
		return 0;
	else
		return -1;
}

#endif // CHANGED

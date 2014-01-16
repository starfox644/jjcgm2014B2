#ifdef CHANGED

#include "semaphoreManager.h"

/**
*	Demande l'initialisation d'un semaphore noyau
*/
int do_SemInit(int adrSem, int initValue);

/**
 * Demande au semaphore identifie par id d'ajouter le thread courant
 * a la liste des threads en attente pour la ressource
 */
int do_SemWait(int id);

/**
 * Demande au semaphore identifie par id de retirer le thread courant
 * de la liste des threads en attente pour la ressource
 */
int do_SemPost(int id);

/**
 * Demande la suppression du semaphore identifie par id
 */
int do_SemDestroy(int id);

#endif

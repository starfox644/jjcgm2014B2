/** Test du TP2 sur la synchronisation **
 * Ce test cree plusieurs threads representant des supporters de 2 equipes.
 * Chaque equipe a un message different que les threads(supporters) ecrivent NB_SONG fois
 * avant de se terminer.
 * Le semaphore sert a conserver le message dans son integralite pour chaque thread,
 * puisqu'il est affiche en 4 fois.
 */

#include "syscall.h"
#define NB_SONG 3

sem_t mutex;

void supporter (void *arg) {
	char *str = (char *) arg ;
	int i;

	for (i = 0; i < NB_SONG; i++){
		SemWait(&mutex);
		PutString("Thread ");
		PutInt(GetTid());
		PutChar(' ');
		PutString(str);
		SemPost(&mutex);
	}
}

int main ()
{
	int team1 = 3;
	int team2 = 5;
	int i ;
	int nb_threads = 0 ;
	int error;
	nb_threads = team1 + team2;
	if((error = SemInit(&mutex, 1)) == -1)
		return -1;

	/* Create the threads for team1 */
	for (i = 0 ; i < team1; i++){
		UserThreadCreate(supporter, "Allons enfants de la patrie...\n") ;
	}
	/* Create the other threads (ie. team2) */
	for ( ; i < nb_threads; i++){
		UserThreadCreate(supporter, "Sweep low, sweet chariot\n") ;
	}

	return 0;
}

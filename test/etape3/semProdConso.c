/**
 * Ce programme lance plusieurs threads et introduit un
 * schema de producteurs consommateurs gere par des semaphores
 * au niveau utilisateur.
 * Source : Cours 4
 */
#ifdef CHANGED
#include "syscall.h"
#define NBMAX 10

char buffer[NBMAX];
int nbElem;
sem_t *mutex, *empty, *full;

void ajouterRessource(char c)
{
	PutString("Debut ajouterRessource\n");
	if (nbElem < NBMAX)
	{
//		PutString("[Prod] Ajout de ");
//		PutChar(c);
//		PutString(" au buffer.\n");
		buffer[nbElem] = c;
		nbElem++;
	}
}


char retirerRessource()
{
	PutString("Debut retirerRessource\n");
	char c  = '0';
	if (0 < nbElem && nbElem < NBMAX)
	{
		nbElem--;
		c = buffer[nbElem];
		PutString("[Cons] Retrait de ");
		PutChar(c);
		PutString(" du buffer.\n");
	}
	return c;
}

void producteur ()
{
	PutString("[Prod] Debut producteur\n");
	while (1)
	{
		PutString("[Prod] Empty pris\n");
		SemWait(*empty);
		PutString("[Prod] Mutex pris\n");
		SemWait(*mutex);
		PutString("[Prod] appel a ajouterRessource\n");
		ajouterRessource('a'+nbElem);
		PutString("[Prod] Mutex relache\n");
		SemPost(*mutex);
		PutString("[Prod] Full relache\n");
		SemPost(*full);
	}
}

void consommateur ()
{
	PutString("[Conso] Debut Consommateur\n");
	while (1)
	{
		PutString("[Conso] Full pris\n");
		SemWait(*full);
		PutString("[Conso] Mutex pris\n");
		SemWait(*mutex);
		PutString("[Conso] appel a retirerRessource\n");
		PutChar(retirerRessource());
		PutString("[Conso] Mutex relache\n");
		SemPost(*mutex);
		PutString("[Conso] Empty relache\n");
		SemPost(*empty);
	}
}

int main ()
{
	int error;
	nbElem = 0;
	PutString("Initialisation des semaphores\n");
	if ((error = SemInit(mutex, 1)) == -1 && (error = SemInit(empty, NBMAX)) == -1 && (error = SemInit(full, 0)) == -1)
		return -1;
	PutString("Creation du thread producteur\n");
	UserThreadCreate(producteur, 0);
	PutString("Creation du thread consommateur\n");
	UserThreadCreate(consommateur, 0);
	return 0;
}
#endif // CHANGED

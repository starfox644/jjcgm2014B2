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
int nbElem, ite;
sem_t mutex, empty, full;

void ajouterRessource(char c)
{
//	PutString("Debut ajouterRessource\n");
	if (nbElem < NBMAX-1)
	{
		buffer[nbElem] = c;
		nbElem++;
	}
}


char retirerRessource()
{
//	PutString("Debut retirerRessource\n");
	char c  = '0';
	if (0 < nbElem && nbElem < NBMAX)
	{
		nbElem--;
		c = buffer[nbElem];
	}
	return c;
}

void producteur ()
{
	PutString("[Prod] Debut producteur\n");
	while (ite < 100)
	{
		SemWait(&empty);
		SemWait(&mutex);
		ajouterRessource('a'+nbElem);
		SemPost(&mutex);
		SemPost(&full);
		ite++;
	}
}

void consommateur ()
{
	PutString("[Conso] Debut Consommateur\n");
	while (ite < 100)
	{
		SemWait(&full);
		SemWait(&mutex);
		PutChar(retirerRessource());
		SemPost(&mutex);
		SemPost(&empty);
		ite++;
	}
}

int main ()
{
	int error;
	nbElem = 0;
	ite = 0;
	PutString("Initialisation des semaphores\n");
	if ((error = SemInit(&mutex, 1)) == -1 || (error = SemInit(&empty, NBMAX)) == -1 || (error = SemInit(&full, 0)) == -1)
		return -1;
	PutString("Creation du thread producteur\n");
	UserThreadCreate(producteur, 0);
	PutString("Creation du thread consommateur\n");
	UserThreadCreate(consommateur, 0);
	return 0;
}
#endif // CHANGED

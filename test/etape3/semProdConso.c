/**
 * Ce programme lance plusieurs threads et introduit un
 * schema de producteurs consommateurs gere par des semaphores
 * au niveau utilisateur
 */
#ifdef CHANGED
#include "syscall.h"
#define NBMAX 10

char buffer[NBMAX];
int nbElem, ite;
int *mutex, id;

void ajouterRessource(char c)
{
	if (nbElem < NBMAX)
	{
		buffer[nbElem] = c;
		nbElem++;
	}
}

char retirerRessource()
{
	char c  = '0';
	if (0 < nbElem && nbElem < NBMAX)
	{
		c = buffer[nbElem];
		nbElem--;
	}
	return c;
}

void producteur ()
{
	while (ite < 100)
	{
		SemWait(*mutex);
		ajouterRessource('a'+nbElem);
		SemPost(*mutex);
		ite++;
	}
}

void consommateur ()
{
	while (ite < 100)
	{
		SemWait(*mutex);
		PutChar(retirerRessource());
		SemPost(*mutex);
		ite++;
	}
}

int main ()
{
	int error;
	ite = 0;
	nbElem = 0;
	mutex = &id;
	PutString("Initialisation du semaphore mutex\n");
	if ((error = SemInit(mutex, 1)) == -1)
		return -1;
	PutString("Creation du thread producteur");
	UserThreadCreate(producteur, 0);
	PutString("Creation du thread consommateur");
	UserThreadCreate(consommateur, 0);
	return 0;


}
#endif // CHANGED

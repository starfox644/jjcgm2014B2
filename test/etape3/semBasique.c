/**
 * Ce programme lance 2 threads.
 * Le 1er verrouille un semaphore et demande une chaine de caractere
 * qu'il va ensuite afficher.
 * Le 2e verouille le meme semaphore et affiche "Mutex pris".
 */
#ifdef CHANGED
#include "syscall.h"
#define NBMAX 10

sem_t mutex;


void producteur ()
{
	char s[16];
	PutString("Debut Producteur\n");
	SemWait(&mutex);
	PutString("Producteur :");
	GetString(s, 16);
	PutString(s);
	SemPost(&mutex);
}

void consommateur ()
{
//	char s[16];
	PutString("Debut Consommateur\n");
	SemWait(&mutex);
	PutString("Mutex pris\n");
	SemPost(&mutex);
}

int main ()
{
	int error;
	PutString("Initialisation des semaphores\n");
	if ((error = SemInit(&mutex, 1)) == -1)
		return -1;
	PutString("Creation du thread producteur\n");
	UserThreadCreate(producteur, 0);
	PutString("Creation du thread consommateur\n");
	UserThreadCreate(consommateur, 0);
	return 0;
}
#endif // CHANGED

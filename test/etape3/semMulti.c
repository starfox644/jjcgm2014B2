/**
 * Ce programme lance plusieurs threads pour tester l'entrelacement des semaphores.
 * 2 threads se partagent un meme semaphore et affiche des chaines de caracteres
 * (semBasique mais avec plusieurs paires de threads associees a des semaphores differents)
 */
#ifdef CHANGED
#include "syscall.h"
#define NBMAX 10

sem_t mutex1, mutex2, mutex3;


void producteur1 ()
{
	char s[16];
	PutString("Debut Producteur1\n");
	SemWait(&mutex1);
	PutString("Producteur1 :\n");
	GetString(s, 16);
	PutString(s);
	SemPost(&mutex1);
}

void consommateur1 ()
{
	PutString("Debut Consommateur1\n");
	SemWait(&mutex1);
	PutString("Mutex1 pris\n");
	SemPost(&mutex1);
}

void producteur2 ()
{
	char s[16];
	PutString("Debut Producteur2\n");
	SemWait(&mutex2);
	PutString("Producteur2 :\n");
	GetString(s, 16);
	PutString(s);
	SemPost(&mutex2);
}

void consommateur2 ()
{
	PutString("Debut Consommateur2\n");
	SemWait(&mutex2);
	PutString("Mutex2 pris\n");
	SemPost(&mutex2);
}

void producteur3 ()
{
	char s[16];
	PutString("Debut Producteur3\n");
	SemWait(&mutex3);
	PutString("Producteur3 :\n");
	GetString(s, 16);
	PutString(s);
	SemPost(&mutex3);
}

void consommateur3 ()
{
	PutString("Debut Consommateur3\n");
	SemWait(&mutex3);
	PutString("Mutex3 pris\n");
	SemPost(&mutex3);
}

int main ()
{
	int error;
	PutString("Initialisation des semaphores\n");
	if ((error = SemInit(&mutex1, 1)) == -1 || (error = SemInit(&mutex2, 1)) == -1 || (error = SemInit(&mutex3, 1)) == -1)
		return -1;
	PutString("Creation du thread producteur1\n");
	UserThreadCreate(producteur1, 0);
	PutString("Creation du thread producteur2\n");
	UserThreadCreate(producteur2, 0);
	PutString("Creation du thread producteur3\n");
	UserThreadCreate(producteur3, 0);
	PutString("Creation du thread consommateur1\n");
	UserThreadCreate(consommateur1, 0);
	PutString("Creation du thread consommateur2\n");
	UserThreadCreate(consommateur2, 0);
	PutString("Creation du thread consommateur3\n");
	UserThreadCreate(consommateur3, 0);
	return 0;
}
#endif // CHANGED

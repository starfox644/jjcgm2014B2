#include "syscall.h"

/*
 *  Teste la creation de thread avec une adresse d'argument nulle.
 *  Une adresse d'argument nulle ne leve jamais une erreur car cela
 *  signifie une absence d'argument.
 *  Le thread est cense etre cree et ce test doit afficher "Thread 2".
 */
void f(void *a)
{
	PutString("Thread 2\n");
	UserThreadExit(0);
}

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadNullArg : \n");
	PutString("Lance un thread avec une adresse d'argument nulle.\n");
	PutString("Doit afficher \"Thread 2\".\n");
	PutString("-----------------------------------------\n");
	if(UserThreadCreate(f, 0) == -1)
	{
		PutString("Erreur de creation de thread.\n");
	}
	else
	{
		PutString("Creation reussie.\n");
	}
    return 0;
}

#include "syscall.h"

/*
 * Lancement de deux threads dont un qui cree l'erreur stackOverFlow.
 * On remarque que la terminaison du processus se passe correctement.
 * Des que l'erreur est levee, g se termine instantanement,
 * on ne voit donc pas s'afficher "Fin g"
 */
void f(void *arg)
{
	//PutString("Debut f\n");
	int* i = (int*)arg;
	int j = *i;
	if (*i < 200)
	{
		j = (*i)+1;
		f(&j);
	}
}

void g(void *arg)
{
	int i;
	PutString("debut g\n");
	for ( i  = 0 ; i < 10000 ; i++);
	PutString("fin g\n");
}
int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test testStackOverflow2 : \n");
	PutString("Teste terminaison threads avec le depassement de la pile.\n");
	PutString("-----------------------------------------\n");
	int i = 0;
	int tidf = UserThreadCreate(f, &i);
	int tidg = UserThreadCreate(g, 0);
	if(UserThreadJoin(tidf, 0) == -1)
		PutString("Erreur join\n");

	if(UserThreadJoin(tidg, 0) == -1)
		PutString("Erreur join\n");
	PutString("fin main!\n");
    return 0;
}

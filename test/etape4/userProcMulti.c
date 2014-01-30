/*
 * userProcMulti.c
 *	Ce programme permet de lancer 120 processus du programme userThreadMulti qui va lui meme créer 120 threads.
 *
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_PROC 120

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test userProcMulti : \n");
	PutString("Lance plusieurs processus qui vont lancer chacun plusieurs threads.\n");
	PutString("-----------------------------------------\n");
	int i;
	for(i = 0; i < MAX_PROC; i++)
	{
		if(ForkExec("./etape4/userThreadMulti") == -1)
		{
			PutString("echec de lancement de processus\n");
		}
		else
		{
			PutString("Processus lance\n");
		}
	}
	return 0;
}


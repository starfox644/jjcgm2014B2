/*
 * userProcMulti.c
 *	Ce programme permet de lancer 12 processus du programme userThreadMulti qui va lui meme créée 12 thread.
 *	Le but est d'avoir 12 processus lancer par notre programme de test qui crée eux meme 12 threads.
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_PROC 14

//sem_t sem;
int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test userProcMulti : \n");
	PutString("Lance plusieurs processus qui vont lancer chacun plusieurs threads.\n");
	PutString("-----------------------------------------\n");
	int i;
	/*int error;
	if((error = SemInit(&sem,1)) == -1)
	{
		return -1;
	}
	SemWait(&sem);*/
	//SemPost(&sem);
	for(i = 0; i < MAX_PROC; i++)
	{
//		PutString("Lancement du proc numéro ");
		//SemWait(&sem);
		//PutInt(i);
		//PutString("\n");
		//SemPost(&sem);
		if(ForkExec("./etape4/userThreadMulti") == -1)
		{
			//SemWait(&sem);
			PutString("echec de lancement de processus\n");
			//SemPost(&sem);
		}
	}
	return 0;
}


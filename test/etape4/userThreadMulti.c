/*
 * userThreadMulti.c
 *	Programme qui crée 12 threads et qui attend qu'il ce termine
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_THREAD 120

sem_t sem;

void f(void* arg)
{
	SemWait(&sem);
	PutString("Lancement du thread\n");
	PutChar('a');
	PutChar('b');
	PutChar('c');
	PutChar('d');
	PutChar('\n');
	SemPost(&sem);
}

int main(){

	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test userThreadMulti : \n");
	PutString("Lance plusieurs threads qui affice chacun \"abcd\".\n");
	PutString("-----------------------------------------\n");
	int i, error;
	if((error = SemInit(&sem,1)) == -1){
		return -1;
	}

	for(i = 0; i < MAX_THREAD; i++)
	{
		if(UserThreadCreate(f, 0) == -1)
		{
			PutString("Erreur de lancement de thread\n");
		}
	}
	return 0;
}




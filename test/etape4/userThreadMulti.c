/*
 * userThreadMulti.c
 *	Programme qui crée 12 threads et qui attend qu'il ce termine
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_THREAD 12

sem_t sem;

void f(void* arg)
{
	//int i;
	SemWait(&sem);
	//i = GetPid();
	PutString("Lancement du thread pid = ");
	//PutInt(i);
	PutChar('a');
	PutChar('b');
	PutChar('c');
	PutChar('d');
	PutChar('\n');
	SemPost(&sem);
	//PutString("Fin du thread\n");
}

int main(){

	int i, error;
	if((error = SemInit(&sem,1)) == -1){
		return -1;
	}

	//SemWait(&sem);
	PutString("\n Lancement du programme de test userThreadMulti\n");
	//SemPost(&sem);
	for(i = 0; i < MAX_THREAD; i++)
	{
		if(UserThreadCreate(f, 0) == -1)
		{
			//SemWait(&sem);
			PutString("Erreur de lancement de thread\n");
			//SemPost(&sem);
		}
		/*else
		{
			PutString("Lancement ");
			PutInt(i);
			PutString("\n");
		}*/
	}
	return 0;
}




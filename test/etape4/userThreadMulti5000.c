/*
 * userThreadMulti.c
 *	Programme qui crée 5000 threads et qui attend qu'ils se terminent
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_THREAD 5000

sem_t sem;

void f(void* arg)
{
	SemWait(&sem);
	PutString("\nPid : ");
	PutInt(GetPid());
	PutString(" | Tid : ");
	PutInt(GetTid());
	PutChar('\n');
	SemPost(&sem);
}

int main(){

	//int pthreadID[MAX_THREAD],
	int i, error;
	if((error = SemInit(&sem,1)) == -1){
		return -1;
	}

	PutString("\n Lancement du programme de test userThreadMulti\n");
	for(i = 0; i < MAX_THREAD; i++){
		UserThreadCreate(f, 0);
	}
	return 0;
}




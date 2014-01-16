/*
 * userThreadMulti.c
 *	Programme qui crée 12 threads et qui attend qu'il ce termine
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_THREAD 6

sem_t sem;

void f(void* arg)
{
	int i = (int) arg;
	SemWait(&sem);
	PutString("Lancement du thread\n");
	PutChar('a');
	PutChar('b');
	PutChar('c');
	PutChar('d');
	PutChar('\n');
	PutInt(i);
	SemPost(&sem);
}

int main(){

	int i, error;
	if((error = SemInit(&sem,1)) == -1){
		return -1;
	}
	PutString("\n Lancement du programme de test userThreadMulti\n");
	for(i = 0; i < MAX_THREAD; i++){
		UserThreadCreate(f, (void*) i);
	}
	return 0;
}



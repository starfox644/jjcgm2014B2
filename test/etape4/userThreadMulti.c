/*
 * userThreadMulti.c
 *
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_THREAD 12

void f(void* arg)
{
	int i;
	for (i = 0 ; i < 80 ; i++);
	PutChar('a');
	PutChar('b');
	PutChar('c');
	PutChar('d');

}

int main(){

	int pthreadID[MAX_THREAD],i;
	PutString("Lancement du programme de test userProcMulti\n");
	for(i = 0; i < MAX_THREAD; i++){
		PutString("Lancement du thread numéro : ");
		PutInt(i);
		PutString("\n");
		pthreadID[i] = UserThreadCreate(f, 0);

	}
	for(i = 0; i < MAX_THREAD; i ++){
		if(UserThreadJoin(pthreadID[i], 0) == -1)
			PutString("Erreur join du main sur le thread : \n");
			PutInt(pthreadID[i]);
			PutString("\n");
	}
	return 0;
}




/*
 * 	progWait.c
 *	Lance une boucle pour attendre un certain temps afin de tester le waitpid via waitPidMulti
 *  Created on: 17 janv. 2014
 *      Author: Sarkal
 */
#include "syscall.h"
#define CPT 50000

int main(){
	int i;
	for(i = CPT; i >= 0; i--)
	{
		if (i%(CPT/5) == 0 && (i/(CPT/5)) != 5)
		{
			PutInt(i/(CPT/5));
			PutChar(' ');
		}
	}
	PutChar('\n');
	return 0;
}


/*
 * 	progWait.c
 *	Lance une boucle pour attendre un certain temps afin de tester le waitpid via waitPidMulti
 *  Created on: 17 janv. 2014
 *      Author: Sarkal
 */
#include "syscall.h"

int main(){
	int i;
	for(i = 0; i < 500000; i++);
	return 0;
}


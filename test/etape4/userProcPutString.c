/*
 * userProc.c

 *
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"


int main()
{
	int pid = 0;
	PutString("\n");
	PutString("Lancement de putstring\n");
	pid = ForkExec("../etape4/pustring");
	if(pid == -1){
		PutString("Erreur lors du ForkExec\n");
	}
	PutString("\n");
    return 0;
}


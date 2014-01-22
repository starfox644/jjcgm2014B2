/*
 * userProc.c

 *
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"


int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test userProcPutString : \n");
	PutString("Cree un processus qui affiche une chaine de caractere.\n");
	PutString("-----------------------------------------\n");
	int pid = 0;
	PutString("\n");
	PutString("Lancement de putstring\n");
	pid = ForkExec("../etape4/pustring");
	if(pid == -1){
		PutString("Erreur lors du ForkExec\n");
		return -1;
	}
	PutString("\n");
    return 0;
}


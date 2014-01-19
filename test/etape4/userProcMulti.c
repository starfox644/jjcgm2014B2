/*
 * userProcMulti.c
 *	Ce programme permet de lancer 12 processus du programme userThreadMulti qui va lui meme créée 12 thread.
 *	Le but est d'avoir 12 processus lancer par notre programme de test qui crée eux meme 12 threads.
 *  Created on: 10 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"
#define MAX_PROC 3

int main(){
	int i;
	PutString("Lancement du programme de test userProcMulti\n");
	for(i = 0; i < MAX_PROC; i++){
		PutString("Lancement du proc numéro ");
		PutInt(i);
		PutString("\n");
		ForkExec("./etape4/userThreadMulti");
		/*PutString("Apres forkExec du proc");
		PutInt(i);
		PutString("\n");*/
	}
	return 0;
}


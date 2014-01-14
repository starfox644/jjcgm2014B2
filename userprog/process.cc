#ifdef CHANGED
#include "process.h"

do_forkExec(char s*) {
	t = new thread(...);
	addProcess;
	t->fork(startProcess, executable);
}

void addProcess () {
	nbProcess++;
}

void removeProcess () {
	nbProcess--;
}

int getNbProcess () {
	return nbProcess;
}

#endif

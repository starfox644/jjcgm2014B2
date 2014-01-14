#ifdef CHANGED
#include "process.h"

do_forkExec(char s*) {
	t = new thread(...);
	addProcess;
	t->fork(startProcess, executable);
}

static void addProcess () {
	nbProcess++;
}

static void removeProcess () {
	nbProcess--;
}

static int getNbProcess () {
	return nbProcess;
}

#endif

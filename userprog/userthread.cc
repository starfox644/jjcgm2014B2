#ifdef CHANGED
#include "userthread.h"
#include "thread.h"

/**
*    f : 		function address to execute in the MIPS processor
*    arg : 	argument address for the f function, f will be called with arg
*/
int do_UserThreadCreate(int f, int arg)
{
	// we create a function_t containing the function f and it’s arguments
	/*function_t fun;
	fun.f = f;
	fun.arg = arg;*/

	Thread *newThread = new Thread("test");
	// creation of the thread, init and positionning in the file
	// the new thread executes StartUserThread (saving register)
	newThread->Fork(StartUserThread, 0);
	return 0;
}

void StartUserThread(int f)
{
	printf("lancement du thread dans start user thread\n");
}

#endif

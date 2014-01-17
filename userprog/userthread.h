#ifdef CHANGED

// decalage en nombre de pages de la pile par rapport au code
#define STACK_OFFSET 2
// adresse de debut de la fonction startThread (dans start.s)
#define THREAD_START_OFFSET 12

class Semaphore;

/**
 * 		Implements system call UserThreadCreate.
 *    f : 		function address to execute in the MIPS processor
 *    arg : 	argument address for the f function, f will be called with arg
 */
int do_UserThreadCreate(int f, int arg);

/**
 * 		Launch an user thread, which have to execute the user function at the address f.
 */
static void StartUserThread(int f);

/**
 * 		Implements system call UserThreadJoin.
 *		Wait the thread tid and store the return code at addrUser.
 *		If the thread isn't found in the process, -1 is returned.
 */
int do_UserThreadJoin(int tid, int addrUser);

/**
 * 		Implements system call UserThreadExit.
 * 		status is given at a thread which wait for it with UserThreadJoin.
 */
void do_UserThreadExit(int status);

Semaphore *s_create;

#endif

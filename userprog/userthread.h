#ifdef CHANGED

// decalage en nombre de pages de la pile par rapport au code
#define STACK_OFFSET 2

/**
*    f : 		function address to execute in the MIPS processor
*    arg : 	argument address for the f function, f will be called with arg
*/
int do_UserThreadCreate(int f, int arg);
static void StartUserThread(int f);
void do_UserThreadExit();

#endif

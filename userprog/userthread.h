#ifdef CHANGED
//structure to contain a function and it’s arguments
typedef struct function
{
int f;
int arg;
} function_t;

/**
*    f : 		function address to execute in the MIPS processor
*    arg : 	argument address for the f function, f will be called with arg
*/
int do_UserThreadCreate(int f, int arg);
void StartUserThread(int f);

#endif

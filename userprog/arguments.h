/*
 * arguments.h
 *
 * structures to use arguments lists
 * f(int n, ...)
 */

#ifdef step4

typedef struct arg_list {
	int next;
	int args[3];
} arg_list;

void do_arg_start(int a, int b, int c);
int do_arg_arg();

#endif

/*
 * arguments.cc
 *
 * functions to use arguments lists
 * f(int n, ...)
 */

#ifdef step4

#include "arguments.h"

arg_list liste;

void do_arg_start(int a, int b, int c) {
	liste.args[0] = a;
	liste.args[1] = b;
	liste.args[2] = c;
	liste.next = 0;
}

int do_arg_arg() {
	int result;

	result = liste.args[liste.next];
	liste.next++;

	return result;
}


#endif

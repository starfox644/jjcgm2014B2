/*
 * arguments.h
 *
 * structures to use arguments lists
 * f(int n, ...)
 */

#ifdef step4

#ifndef _ARGUMENTS_H_
#define _ARGUMENTS_H_

#include "list.h"

typedef struct arg_list_t {
	int nb;
	int args[3];
	int next;
} arg_list_t;

typedef int arg_list;

arg_list do_arg_start(int a, int b, int c);
int do_arg_arg(arg_list arg);

#endif

#endif

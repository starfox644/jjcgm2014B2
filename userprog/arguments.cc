/*
 * arguments.cc
 *
 * functions to use arguments lists
 * f(int n, ...)
 */

#ifdef step4

#include "arguments.h"
#include <list>

int nbNext = 1;
std::list<arg_list_t> listeArg;

arg_list do_arg_start (int a, int b, int c) {
	arg_list_t new_arg_list;
	int nb;


	new_arg_list.nb = nbNext;
	new_arg_list.args[0] = a;
	new_arg_list.args[1] = b;
	new_arg_list.args[2] = c;
	new_arg_list.next = 0;

	listeArg.push_back(new_arg_list);

	nb = nbNext;

	nbNext++;

	return nb;
}

int do_arg_arg(arg_list arg) {
	int nb;
	std::list<arg_list_t>::iterator it=listeArg.begin();

	while (it != listeArg.end() && it->nb != arg)
		it++;

	// if not found
	if(it == listeArg.end())
		return -1;

	// If arg not found, return -1 : error
	if (it->nb != arg)
		return -1;
	// Else, return it
	else {
		nb = it->next;
		it->next++;
		return (int)it->args[nb];
	}

	return 0;
}

int do_arg_end(arg_list arg) {
	std::list<arg_list_t>::iterator it=listeArg.begin();

	while (it != listeArg.end() && it->nb != arg)
		it++;

	// if not found
	if(it == listeArg.end())
		return -1;

	// If arg not found, return -1 : error
	if (it->nb != arg)
		return -1;
	// Else, return it
	else {
		listeArg.erase(it);
	}

	return 0;
}

#endif

/*
 * util.h
 *
 *  Created on: 18 janv. 2014
 *      Author: galdween
 */

#ifndef UTIL_H_
#define UTIL_H_
#include "String.h"
#include "syscall.h"
#include "Printf.h"
#include "../malloc/malloc_stub.h"

/*
 * Cette fonction permet de transformer un string en entier
 */
int Atoi(char* variable);

/*
 * Cette fonction permet de transformer un entier en string
 */
char* Itoa(int variable);

#endif /* UTIL_H_ */

/*
 * eval.h
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */
#ifndef __eval__h__
#define __eval__h__

#include "job.h"
#include "../nachos_libc/String.h"
#include "syscall.h"
#include "../nachos_libc/Printf.h"
#include "parseline.h"

#define MAX_CHAR 128
#define MAXARGS 128
//fonction
void evalLine(char*);
int commandIntegre(char**);

#endif

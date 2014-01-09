/*
 * getstringNull.c
 *
 *  Created on: 8 janv. 2014
 *      Author: galdween
 */
#include "syscall.h"

int main()
{
	if(GetString(0,10) == -1)
		PutString("Erreur de GetString");

	return 0;
}

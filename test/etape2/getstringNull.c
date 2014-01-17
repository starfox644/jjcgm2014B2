/*
 * getstringNull.c
 *
 *  Created on: 8 janv. 2014
 *      Author: galdween
 *
 *  Effectue un appel systeme GetString en passant une adresse nulle
 */
#include "syscall.h"

int main()
{
	if(GetString(0,10) == -1)
		PutString("Erreur de GetString");

	return 0;
}

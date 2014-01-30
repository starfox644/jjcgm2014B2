//  Effectue un appel systeme GetString en passant une adresse nulle
#include "syscall.h"

int main()
{
	if(GetString(0,10) == -1)
		PutString("Erreur de GetString\n");

	return 0;
}

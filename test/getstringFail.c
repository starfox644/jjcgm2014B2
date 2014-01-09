#include "syscall.h"

int main()
{
	char s[5];

	// on essaye de recuperer 50 caracteres max dans un tableau de taille 5
	if(GetString(s, 50) != -1)
		PutString(s);
	else
		PutString("Erreur de GetString");

	return 0;
}

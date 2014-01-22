#include "syscall.h"

// affiche la chaine hello world
int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test putstring : Affiche \"Hello World !\".\n");
	PutString("-----------------------------------------\n");
	// System call PutString test
	PutString("Hello World !\n");
    return 0;
}

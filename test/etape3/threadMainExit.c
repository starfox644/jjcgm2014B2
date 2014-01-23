#include "syscall.h"

/**
 * 	Permet de tester le comportement du programme lorsque le thread principal
 * 	appelle UserThreadExit avec une valeur de retour.
 */

int main()
{
	PutString("\n-----------------------------------------\n");
	PutString("Lancement du test threadMainExit : \n");
	PutString("Test l'appel de UserThreadExit par le main avec une valeur de retour.\n");
	PutString("-----------------------------------------\n");
	UserThreadExit(5);
    return 0;
}

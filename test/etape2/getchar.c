#include "syscall.h"

int main()
{
	int c;
	PutString("Demande un caractere jusqu'a recevoir un Ctrl+D\n");
    // on récupere des caracteres et on les affiche tant qu'on a pas de fin de fichier
    while((c = GetChar()) != -1)
    	PutChar(c);
    return 0;
}

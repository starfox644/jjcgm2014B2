#include "syscall.h"

int main()
{
	int c;
    // on récupere des caracteres et on les affiche tant qu'on a pas de fin de fichier
    while((c = GetChar()) != -1)
    	PutChar(c);
    return 0;
}

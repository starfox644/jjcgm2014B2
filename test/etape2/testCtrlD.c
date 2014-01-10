#include "syscall.h"

// lit des caracteres et les ecrit jusqu'a lire une fin de fichier
int main()
{
	int c;
	while((c = GetChar()) != -1)
		PutChar((char)c);
    return 0;
}

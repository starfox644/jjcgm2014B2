#include "syscall.h"

int main()
{
    char c;

    // on récupere des caracteres et on les affiche tant qu'on a pas de 'q'
    do {
    	c = (char) GetChar();
    	PutChar(c);
    } while (c != 'q');
    return 0;
}

#include "syscall.h"

/**
 * Affiche n caracteres a partir du caractere c
 * ex print(f, 3) affichera "fgh"
 */
void print(char c, int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        PutChar(c+i);
    }
    PutChar('\n');
}

int main()
{
    print('a', 4);
    return 0;
}

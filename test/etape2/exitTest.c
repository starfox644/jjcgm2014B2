#include "syscall.h"

// ecrit abcd et renvoie 6 en valeur de retour
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

    return 6;
}

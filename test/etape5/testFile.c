#include "syscall.h"

int main()
{
	OpenFileId id;
	char c;
	int n;
	id = Open("test");
	if(id == -1)
	{
		PutString("erreur open\n");
		Exit(0);
	}
	if(Write("coucou\n", 7, id) != 7)
	{
		PutString("Erreur write\n");
		Exit(0);
	}
	if(Close(id) == -1)
	{
		PutString("erreur close\n");
		Exit(0);
	}

	id = Open("small");
	if(id == -1)
	{
		PutString("erreur open\n");
		Exit(0);
	}
	while((n = Read(&c, sizeof(char), id)))
	{
		PutChar(c);
	}
	if(n == -1)
	{
		PutString("Erreur read\n");
		Exit(0);
	}
	if(Close(id) == -1)
	{
		PutString("erreur close\n");
		Exit(0);
	}
    return 0;
}

#include "syscall.h"

int main()
{
	OpenFileId id;
	char c;
	int n;
	if(Create("test", 128) == -1)
	{
		PutString("Erreur create\n");
		Exit(0);
	}
	id = Open("test");
	if(id == -1)
	{
		PutString("erreur open test\n");
		Exit(0);
	}
	if(Write("coucou\n", 7, id) != 7)
	{
		PutString("erreur write\n");
		Exit(0);
	}
	if(Close(id) == -1)
	{
		PutString("erreur close test\n");
		Exit(0);
	}

	id = Open("files/small");
	if(id == -1)
	{
		PutString("erreur open small\n");
		Exit(0);
	}
	while((n = Read(&c, sizeof(char), id)))
	{
		PutChar(c);
	}
	if(n == -1)
	{
		PutString("erreur read\n");
		Exit(0);
	}
	if(Close(id) == -1)
	{
		PutString("erreur close small\n");
		Exit(0);
	}
    return 0;
}

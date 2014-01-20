#include "../malloc/malloc_stub.h"
#include "syscall.h"
#define SIZE 20

int main()
{
	char* buffer = (char*)malloc(SIZE * sizeof(char));
	if(buffer == 0)
	{
		PutString("Erreur de malloc.\n");
	}
	else
	{
		GetString(buffer, SIZE);
		PutString(buffer);
	}
	free(buffer);
	return 0;
}

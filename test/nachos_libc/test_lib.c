#include "syscall.h"
#include "../nachos_libc/Printf.h"
#include "../nachos_libc/util.h"
#include "../malloc/malloc_stub.h"

/**
 *  Teste les fonctions de la librairie utilisateur.
 */

#define SIZE 128

int main()
{
	int i;
	char* buffer = (char*)malloc(SIZE * sizeof(char));
	GetString(buffer, SIZE);
	i = Atoi(buffer);
	Printf("entier recupere : %i\n", i);
	StrCpy(Itoa(42), buffer);
	Printf("entier converti : %s\n", buffer);
	free(buffer);
	return 0;
}

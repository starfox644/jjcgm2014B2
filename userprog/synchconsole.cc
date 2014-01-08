#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;

static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }

static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar (ch);
	writeDone->P ();		// wait for write to finish
}

char SynchConsole::SynchGetChar()
{
	char ch;
	readAvail->P ();		// wait for character to arrive
	ch = console->GetChar ();
return  ch;
}

void SynchConsole::SynchPutString(const char s[])
{
	int i = 0;

	// on traite chaque caractere jusqu'au caractere '\0'
	while (s[i] != '\0') {
		console->PutChar (s[i]);
		writeDone->P ();
		i++;
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	// ...
}

/**
*	from : MIPS string adress in its memory
*	to : nachOs string adress where it will be saved, must have size+1 chars
*          size : maximum number of characters
*/
void copyStringFromMachine(int from, char *to, unsigned size)
{
	unsigned int i = 0;
	int result;
	// While there’s char to read, we read byte by byte and put it in array to
	while (i < size && machine->ReadMem(from+i, 1, &result)) {
		to[i] = (char) result;
		i++;
	}
	to[i] = '\0';
}

#endif // CHANGED

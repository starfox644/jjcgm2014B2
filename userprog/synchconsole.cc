#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;

static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }

static void WriteDone(int arg) { writeDone->V(); }

/**
 * SynchConsole constructor
 */
SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

/**
 * SynchConsole destructor
 */
SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

/**
 * Print the character given in parameter
 */
void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar (ch);
	writeDone->P ();		// wait for write to finish
}

/**
 * Ask a character in the console
 */
char SynchConsole::SynchGetChar()
{
	char ch;
	readAvail->P ();		// wait for character to arrive
	ch = console->GetChar ();

	return  ch;
}

/**
 * Print the string given in parameter
 * This function suppose that 's' is a valid string adress
 */
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

/**
 * Ask for a string in the console
 */
void SynchConsole::SynchGetString(char *s, int n)
{
	int i = 0;
	char ch;

	while (i < n-1 && ch != '\n' && ch != EOF) {
		// wait for character to arrive
		readAvail->P ();
		ch = console->GetChar ();

		if (ch != '\n' && ch != EOF) {
			s[i] = ch;
			i++;
		}
	}

	s[i] = '\0';
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

/**
 *  from : string adress in memory where the string read was saved
 *  to : buffer adress in MIPS
 */
void copyStringToMachine(char* from, int to)
{
	int i = 0;
	while(from[i] != '\0')
	{
		machine->WriteMem(to, 1, (int)from[i]);
	}
}

#endif // CHANGED

#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;

static Semaphore *writeDone;

static Semaphore *semRead;
static Semaphore *semWrite;

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

	semRead = new Semaphore("Reading semaphore", 1);
	semWrite = new Semaphore("Writing semaphore", 1);
}

/**
 * SynchConsole destructor
 */
SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
	delete semRead;
	delete semWrite;
}

/**
 * Print the character given in parameter
 */
void SynchConsole::SynchPutChar(const char ch)
{
	semWrite->P ();
	console->PutChar (ch);
	writeDone->P ();		// wait for write to finish
	semWrite->V ();
}

/**
 * Ask a character in the console
 */
int SynchConsole::SynchGetChar()
{
	int ch;

	semRead->P ();

	readAvail->P ();		// wait for character to arrive
	ch = console->GetChar ();

	semRead->V ();
	return  ch;
}

/**
 * Print the string given in parameter
 * This function suppose that 's' is a valid string adress
 */
int SynchConsole::SynchPutString(const char s[])
{
	int i = 0;

	semWrite->P ();

	// on traite chaque caractere jusqu'au caractere '\0'
	while (s[i] != '\0') {
		console->PutChar (s[i]);
		writeDone->P ();
		i++;
	}

	semWrite->V ();
	return i;
}

/**
 * Ask for a string in the console
 */
int SynchConsole::SynchGetString(char *s, int n)
{
	int i = 0;
	int ch = 0;

	semRead->P ();

	while (i < n-1 && (char)ch != '\n' && ch != EOF)
	{

		// wait for character to arrive
		readAvail->P ();
		ch = console->GetChar ();

		if (ch != EOF)
		{
			s[i] = (char)ch;
			i++;
		}
	}
	s[i] = '\0';

	semRead->V ();

	return i;

}

/**
 *	Print the interger given in parameter
 *	return nb characters write
 */
int SynchConsole::SynchPutInt(int n)
{
	int nb = 0;
	char str[MAX_STRING_SIZE];

	nb = snprintf(str, MAX_STRING_SIZE, "%d", n);
	synchconsole->SynchPutString(str);

	return nb;
}

/**
 *	from : MIPS string adress in its memory
 *	to : nachOs string adress where it will be saved, must have size+1 chars
 *          size : maximum number of characters
 *  returns false on memory error
 */
bool copyStringFromMachine(int from, char *to, unsigned size)
{
	unsigned int i = 0;
	int result;
	bool err = true;
	// While there’s char to read, we read byte by byte and put it in array to
	while (i < size && (err = machine->ReadMem(from+i, 1, &result)) && (char)result != '\0') {
		to[i] = (char) result;
		i++;
	}
	to[i] = '\0';
	return err;
}

/**
 *  from : string adress in memory where the string read was saved
 *  to : buffer adress in MIPS
 *  returns false on memory error
 */
bool copyStringToMachine(char* from, int to)
{
	int i = 0;
	bool err = true;
	while(from[i] != '\0' && (err = machine->WriteMem(to+i, 1, (int)from[i])))
	{
		i++;
	}
	return err;
}


#endif // CHANGED

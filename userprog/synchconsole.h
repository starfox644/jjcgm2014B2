#ifdef CHANGED

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include <pthread.h>

#include "copyright.h"
#include "utility.h"
#include "console.h"
#include "synch.h"

#define Printf(args...) \
	synchconsole->semWrite->P(); \
    printf(args); \
    synchconsole->semWrite->V(); \

class SynchConsole {
public:
	SynchConsole(char *readFile, char *writeFile);

// initialize the hardware console device
	~SynchConsole();// clean up console emulation

	void SynchPutChar(const char ch);// Unix putchar(3S)

	int SynchGetChar();// Unix getchar(3S)

	int SynchPutString(const char *s);// Unix puts(3S)
	int SynchGetString(char *s, int n);// Unix fgets(3S)

	/**
	 *	Print the interger given in parameter
	 *	return nb characters write
	 */
	int SynchPutInt(int n);

	Semaphore *semRead;
	Semaphore *semWrite;
private:
	Console *console;
};

/**
 *	from : MIPS string adress in its memory
 *	to : nachOs string adress where it will be saved, must have size+1 chars
 *          size : maximum number of characters
 */
 bool copyStringFromMachine(int from, char *to, unsigned size);

 /**
  *  from : string adress in memory where the string read was saved
  *  to : buffer adress in MIPS
  */
 bool copyStringToMachine(char* from, int to);

#endif // SYNCHCONSOLE_H

#endif // CHANGED

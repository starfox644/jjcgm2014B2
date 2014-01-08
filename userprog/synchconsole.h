#ifdef CHANGED

#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole {
public:
	SynchConsole(char *readFile, char *writeFile);

// initialize the hardware console device
	~SynchConsole();// clean up console emulation

	void SynchPutChar(const char ch);// Unix putchar(3S)

	char SynchGetChar();// Unix getchar(3S)

	void SynchPutString(const char *s);// Unix puts(3S)
	void SynchGetString(char *s, int n);// Unix fgets(3S)


private:
	Console *console;
};

/**
 *	from : MIPS string adress in its memory
 *	to : nachOs string adress where it will be saved, must have size+1 chars
 *          size : maximum number of characters
 */
 void copyStringFromMachine(int from, char *to, unsigned size);

 /**
  *  from : string adress in memory where the string read was saved
  *  to : buffer adress in MIPS
  */
 void copyStringToMachine(char* from, int to);

#endif // SYNCHCONSOLE_H

#endif // CHANGED

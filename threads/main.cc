// main.cc 
//      Bootstrap code to initialize the operating system kernel.
//
//      Allows direct calls into internal operating system functions,
//      to simplify debugging and testing.  In practice, the
//      bootstrap code would just initialize data structures,
//      and start a user program to print the login prompt.
//
//      Most of this file is not needed until later assignments.
//
// Usage: nachos -d <debugflags> -rs <random seed #>
//              -s -x <nachos file> -c <consoleIn> <consoleOut>
//              -f -cp <unix file> <nachos file>
//              -p <nachos file> -r <nachos file> -l -D -t
//              -n <network reliability> -m <machine id>
//              -o <other machine id>
//              -z
//
//    -d causes certain debugging messages to be printed (cf. utility.h)
//    -rs causes Yield to occur at random (but repeatable) spots
//    -z prints the copyright message
//
//  USER_PROGRAM
//    -s causes user programs to be executed in single-step mode
//    -x runs a user program
//    -c tests the console
//
//  FILESYS
//    -f causes the physical disk to be formatted
//    -cp copies a file from UNIX to Nachos
//    -p prints a Nachos file to stdout
//    -r removes a Nachos file from the file system
//    -l lists the contents of the Nachos directory
//    -D prints the contents of the entire file system 
//    -t tests the performance of the Nachos file system
//
//  NETWORK
//    -n sets the network reliability
//    -m sets this machine's host id (needed for the network)
//    -o runs a simple test of the Nachos network software
//
//  NOTE -- flags are ignored until the relevant assignment.
//  Some of the flags are interpreted here; some in system.cc.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#define MAIN
#include "copyright.h"
#undef MAIN

#include "utility.h"
#include "system.h"
#ifdef CHANGED
#include "process.h"
#else
extern void StartProcess (char *file);
#endif


// External functions used by this file

extern void ThreadTest (void), Copy (const char *unixFile, const char *nachosFile);
extern void Print (char *file), PerformanceTest (void);

#ifdef step4
	extern int allocatorTest(char* file);
#endif

extern void ConsoleTest (char *in, char *out);
#ifdef CHANGED
	extern void SynchConsoleTest (char *in, char *out);
	extern void moveTest();

#endif
#ifdef NETWORK
	extern void MailTest (int networkID);
#ifdef CHANGED
	extern void testCond();
#endif //CHANGED
#endif//NETWORK
//----------------------------------------------------------------------
// main
//      Bootstrap the operating system kernel.  
//      
//      Check command line arguments
//      Initialize data structures
//      (optionally) Call test procedure
//
//      "argc" is the number of command line arguments (including the name
//              of the command) -- ex: "nachos -d +" -> argc = 3 
//      "argv" is an array of strings, one for each command line argument
//              ex: "nachos -d +" -> argv = {"nachos", "-d", "+"}
//----------------------------------------------------------------------

int
main (int argc, char **argv)
{
	int argCount;		// the number of arguments for a particular command

	DEBUG ('t', "Entering main");
	(void) Initialize (argc, argv);

#ifdef THREADS
	ThreadTest ();
#endif

	for (argc--, argv++; argc > 0; argc -= argCount, argv += argCount)
	{
		argCount = 1;
		if (!strcmp (*argv, "-z"))	// print copyright
			printf ("%s", copyright);
#ifdef USER_PROGRAM
		if (!strcmp (*argv, "-x"))
		{			// run a user program
			ASSERT (argc > 1);
			StartProcess (*(argv + 1));
			return -1;
			argCount = 2;
		}
		else if (!strcmp (*argv, "-c"))
		{			// test the console
#ifdef CHANGED
			StartProcess((char*)"./etape2/getchar");
#else

			if (argc == 1)
				ConsoleTest (NULL, NULL);
			else
			{
				ASSERT (argc > 2);
				ConsoleTest (*(argv + 1), *(argv + 2));
				argCount = 3;
			}
			interrupt->Halt ();	// once we start the console, then
			// Nachos will loop forever waiting
			// for console input
#endif
		}
#ifdef CHANGED
		else if (!strcmp (*argv, "-sc"))
		{
			/*currentProcess = new Process();
			if (argc == 1)
			{
				SynchConsoleTest (NULL, NULL);
			}
			else
			{
				ASSERT (argc > 2);
				SynchConsoleTest (*(argv + 1), *(argv + 2));
				argCount = 3;
			}
			interrupt->Halt ();*/
			StartProcess((char*)"./etape2/getchar");
		}

#endif	//changed
#ifdef step4

		else if(!strcmp (*argv, "-ta"))
		{
			ASSERT (argc > 1);
			allocatorTest(*(argv + 1));
			argCount = 2;
			return 0;
		}
		else if (!strcmp (*argv, "-sh")){
			if(argc == 1){
#ifdef FILESYS
				char program[20] = "./my_shell";
#else
				char program[20] = "./my_shell/my_shell";
#endif
				StartProcess(program);
				return -1;
			}else{
				printf("Erreur\n");
				return -1;
			}
		}


#endif


#endif // USER_PROGRAM
#ifdef FILESYS
#ifdef CHANGED
		currentProcess = new Process();
#endif
		if (!strcmp (*argv, "-cp"))
		{			// copy from UNIX to Nachos
			if(argc > 2)
			{
				Copy (*(argv + 1), *(argv + 2));
				argCount = 3;
			}
			else
			{
				printf("format : -cp <unix file> <nachos file>\n");
			}
		}
		else if (!strcmp (*argv, "-p"))
		{			// print a Nachos file
			if(argc > 1)
			{
				Print (*(argv + 1));
				argCount = 2;
			}
			else
			{
				printf("format : -p <nachos file>\n");
			}
		}
		else if (!strcmp (*argv, "-r"))
		{			// remove Nachos file
			if(argc > 1)
			{
#ifdef CHANGED
				fileSystem->RemoveFile(*(argv + 1));
#else
				fileSystem->Remove (*(argv + 1));
#endif
				argCount = 2;
			}
			else
			{
				printf("format : -r <nachos file>\n");
			}
		}
		else if (!strcmp (*argv, "-l"))
		{			// list Nachos directory
			fileSystem->List ();
		}
		else if (!strcmp (*argv, "-D"))
		{			// print entire filesystem
			fileSystem->Print ();
		}
		else if (!strcmp (*argv, "-t"))
		{			// performance test
			PerformanceTest ();
		}
#ifdef CHANGED
		else if(!strcmp (*argv, "-cr"))
		{
			if(argc > 1)
			{
				fileSystem->CreateDir (*(argv + 1));
				argCount = 2;
			}
			else
			{
				printf("format : -cr <dir name>\n");
			}
		}
		else if(!strcmp(*argv, "-mt"))
		{

			moveTest();
		}
#endif
#endif // FILESYS
#ifdef NETWORK
#ifndef CHANGED
		if (!strcmp (*argv, "-o"))
		{
			ASSERT (argc > 1);
			Delay (5);	// delay for 2 seconds
			// to give the user time to
			// start up another nachos
			MailTest (atoi (*(argv + 1)));
			argCount = 2;
		}
#endif // CHANGED
#ifdef CHANGED
		if(!strcmp(*argv,"-co")){
			testCond();
		}
#endif// CHANGED
#endif // NETWORK
	}

#ifdef CHANGED
	Exit(0);
#else
	currentThread->Finish ();	// NOTE: if the procedure "main"
#endif // CHANGED
	// returns, then the program "nachos"
	// will exit (as any other normal program
	// would).  But there may be other
	// threads on the ready list.  We switch
	// to those threads by saying that the
	// "main" thread is finished, preventing
	// it from returning.
	return (0);			// Not reached...
}

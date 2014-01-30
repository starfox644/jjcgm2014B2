// system.h 
//      All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"
#ifdef step4
#include "frameProvider.h"
#include "processManager.h"
#endif
#ifdef NETWORK
#include "socketManager.h"
#endif

#ifdef CHANGED
#include "syscall.h"
#endif //CHANGED

// Initialization and cleanup routines
extern void Initialize (int argc, char **argv);	// Initialization,
						// called before anything else
extern void Cleanup ();		// Cleanup, called when
						// Nachos is done.

#ifdef CHANGED
extern Process* currentProcess;
extern Semaphore *s_createProcess;

//extern Semaphore *semRead;
//extern Semaphore *semWrite;
#endif

extern Thread *currentThread;	// the thread holding the CPU
extern Thread *threadToBeDestroyed;	// the thread that just finished
extern Scheduler *scheduler;	// the ready list
extern Interrupt *interrupt;	// interrupt status
extern Statistics *stats;	// performance metrics
extern Timer *timer;		// the hardware alarm clock
extern int nextPid;
#ifdef step4
extern FrameProvider *frameProvider; // permet de gerer les cadres
extern ProcessManager *processManager; //permet de gerer les processus
//// number of process for process.cc
#endif
#ifdef USER_PROGRAM
#include "machine.h"
extern Machine *machine;	// user program memory and registers

#ifdef CHANGED
#include "synchconsole.h"
extern SynchConsole *synchconsole;
#endif // CHANGED

#endif

#ifdef FILESYS_NEEDED		// FILESYS or FILESYS_STUB
#include "filesys.h"
extern FileSystem *fileSystem;
#endif

#ifdef step5
#include "filesys.h"
extern FileSystem *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk *synchDisk;
#ifdef CHANGED
extern Semaphore* semFileSys;
#endif
#endif

#ifdef NETWORK
#include "../network/post.h"
extern PostOffice *postOffice;
extern SocketManager *socketManager;
#endif

#endif // SYSTEM_H

/* syscalls.h 
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel. 
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation 
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

/* system call codes -- used by the stubs to tell the kernel which system call
 * is being asked for
 */
#define SC_Halt		0
#define SC_Exit		1
#define SC_Exec		2
#define SC_Join		3
#define SC_Create	4
#define SC_Open		5
#define SC_Read		6
#define SC_Write	7
#define SC_Close	8
#define SC_Fork		9
#define SC_Yield	10

#ifdef CHANGED
#define SC_PutChar	 11
#define SC_PutString 12
#define SC_GetChar	 13
#define SC_GetString 14
#define SC_PutInt	 15
#define SC_UserThreadCreate 16
#define SC_UserThreadExit 17
#define SC_GetTid 18
#define SC_SemInit 19
#define SC_SemWait 20
#define SC_SemPost 21
#define SC_SemDestroy 22
#define SC_UserThreadJoin 23
#define SC_ForkExec 24
#define SC_GetPid 25
#define SC_WaitPid 26
#define SC_Mmap 27
#define SC_Unmap 28
#define SC_ArgStart 29
#define SC_ArgArg 30
#define SC_ArgEnd 31
#define SC_GetListProcess 32
#define SC_GetNbProcessTotal 33
#define SC_Cd 34
#define SC_Pwd 35
#define SC_Mkdir 36
#define SC_Ls 37
#define SC_Rmdir 38
#define SC_PathExists 39
#define SC_Rm 40
#define SC_Random 41
#define SC_Seek 42

#define MAX_STRING_SIZE 1024
#endif //CHANGED

#ifdef IN_USER_MODE

// LB: This part is read only on compiling the test/*.c files.
// It is *not* read on compiling test/start.S


/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the 
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking, 
 * from the system call entry point in exception.cc.
 */

/* Stop Nachos, and print out performance stats */
void Halt () __attribute__((noreturn));


/* Address space control operations: Exit, Exec, and Join */

/* This user program is done (status = 0 means exited normally). */
void Exit (int status) __attribute__((noreturn));

/* A unique identifier for an executing user program (address space) */
typedef int SpaceId;

/* Run the executable, stored in the Nachos file "name", and return the 
 * address space identifier
 */
SpaceId Exec (char *name);

/* Only return once the the user program "id" has finished.  
 * Return the exit status.
 */
int Join (SpaceId id);


/* File system operations: Create, Open, Read, Write, Close
 * These functions are patterned after UNIX -- files represent
 * both files *and* hardware I/O devices.
 *
 * If this assignment is done before doing the file system assignment,
 * note that the Nachos file system has a stub implementation, which
 * will work for the purposes of testing out these routines.
 */

/* A unique identifier for an open Nachos file. */
typedef int OpenFileId;

/* when an address space starts up, it has two open files, representing 
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define ConsoleInput	0
#define ConsoleOutput	1

/* Create a Nachos file, with "name" */
void Create (char *name);

/* Open the Nachos file "name", and return an "OpenFileId" that can 
 * be used to read and write to the file.
 */
OpenFileId Open (char *name);

#ifdef CHANGED
/* Write "size" bytes from "buffer" to the open file.
 * Return the number of bytes written
*/
int Write (char *buffer, int size, OpenFileId id);
#else
/* Write "size" bytes from "buffer" to the open file. */
void Write (char *buffer, int size, OpenFileId id);
#endif

/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int Read (char *buffer, int size, OpenFileId id);

#ifdef CHANGED
int Close (OpenFileId id);
#else
/* Close the file, we're done reading and writing to it. */
void Close (OpenFileId id);
#endif



/* User-level thread operations: Fork and Yield.  To allow multiple
 * threads to run within a user program. 
 */

/* Fork a thread to run a procedure ("func") in the *same* address space 
 * as the current thread.
 */
void Fork (void (*func) ());

/* Yield the CPU to another runnable thread, whether in this address space 
 * or not. 
 */
void Yield ();

#ifdef CHANGED

/**
 * User semaphore
 */
typedef int sem_t;

/*
 * PutChar writes in console the given character
 */
void PutChar(char c);

/*
 * PutString writes in console the given string at the adress
 */
int PutString(char* s);

/**
 * Writes an int in console
 */
int PutInt(int i);

/*
 * GetString writes in s less than n characters read on entry. Stop on new line or EOF.
 * Returns -1 on error
 */
int GetString(char* s, int n);

/**
 * GetChar returns a character which was entered
 */
int GetChar();

/**
 * Create a new user thread
 */
int UserThreadCreate(void f(void *arg), void *arg);

/**
 * Terminate the current thread
 */
void UserThreadExit(int status);

/**
 * 	Return the current thread id
 */
int GetTid();

/**
 *	The current thread wait the given thread
 *	ptr_return : return code address of the current thread
 */
int UserThreadJoin(int tid, int* ptr_return);

/**
 * Create a new user semaphore with a token initialized to initValue
 */
int SemInit(sem_t *adrSem, int initValue);

/**
 * Tell the thread to wait for a resource on the semaphore
 */
int SemWait(sem_t *adrSem);

/**
 * Release the semaphore once the resource is not needed anymore
 */
int SemPost(sem_t *adrSem);

/**
 * Create a thread and execute the program given in parameter
 */
int ForkExec(char *executable);

/**
 * Return the current procesus ID
 */
int GetPid();

/*
 *  attend qu'un processus soit fini pour continuer a executer le processus courant
 */
int WaitPid(int processPid);

/*
 * 	alloue une portion de memoire virtuelle
 */
void* Mmap(int length);

/**
 * 	desalloue une portion de memoire virtuelle
 */
int Unmap(void* addr);

/**
 * fonctions et structures de gestion de liste d'arguments
 * maximum 3 arguments peuvent etres traites
 */
typedef int arg_list;

arg_list arg_start();
int arg_arg(arg_list arg);

/*
 * renvoi le nombre de processus système en cours
 */
int GetNbProcessTotal();

/*
 * renvoi la liste des processus créé
 */
int* GetListProcess(int*);

/*
 * Permet de changer de repertoire courant
 * Renvoie 1 en cas de succes, 0 sinon
 */
int cd(char* path);

/*
 * renvoie le path du repertoire courant
 */
const char* pwd();

/*
 * Cree un nouveau dossier vide dans le repertoire courant
 * Renvoie 1 en cas de succes, 0 sinon
 */
int mkdir(const char *path);

/*
 * Affiche le contenu du repertoire courant
 */
void ls();

/*
 * Supprime le dossier vide path
 * Renvoie 1 en cas de succes, 0 sinon
 */
int rmdir(char *path);

/*
 * Renvoie 1 si path existe, 0 sinon
 */
int pathExists(const char* path);

/*
 * Supprime le fichier path
 * Renvoie 1 en cas de succes, 0 sinon
 */
int rm(char* path);

/*
 * Cree un fichier de taille initialSize et de nom path
 * Renvoie 1 en cas de succes, 0 sinon
 */
int create(const char *path, int initialSize);

/*
 *	Renvoie un entier tire aleatoirement
 */
int Random(int nbMax);

/*
 *	Modification de la position d'ou commencea lire ou ecrire
 */
int Seek(int position, int idFile);

#endif // CHANGED

#endif // IN_USER_MODE

#endif /* SYSCALL_H */

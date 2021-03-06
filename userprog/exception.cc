// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "exit.h"
#include "semaphore.h"
#include "process.h"
#include "machine.h"
#include "processManager.h"
#include "addrSpaceAllocator.h"
#include "arguments.h"
#include "synchconsole.h"
#include "socket.h"
#include "socketManager.h"

#ifdef FILESYS
#include "filemanager.h"
#include "filesys.h"
extern FileManager* fm;
#endif
extern int do_UserThreadCreate(int f, int arg);
extern int do_UserThreadJoin(int tid, int addrUser);
extern void do_UserThreadExit(int status);
extern void Copy(const char *from, const char *to);
extern void Print(char *name);
//extern bool isInStack(int addr);
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
	int pc = machine->ReadRegister (PCReg);
	machine->WriteRegister (PrevPCReg, pc);
	pc = machine->ReadRegister (NextPCReg);
	machine->WriteRegister (PCReg, pc);
	pc += 4;
	machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
	int type = machine->ReadRegister (2);
#ifdef CHANGED
	char ch;
	char buffer[MAX_STRING_SIZE];
	char* dynBuffer;
	int adr,codeErreur, n;
	int maxSize;
#ifdef step3
	bool isSuccess;
#endif //step3
#ifdef NETWORK
	int adrBuffer;
#endif // NETWORK

	if (which == SyscallException)
	{
		switch (type)
		{
			case SC_Halt:
				DEBUG ('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt ();
				break;

			case SC_PutChar:
				// reads R4 (argument) into a character
				ch = (char)machine->ReadRegister(4);
				// writes the character
				synchconsole->SynchPutChar(ch);
				break;

			case SC_GetChar:
				ch = synchconsole->SynchGetChar();
				// writes the character into R2
				machine->WriteRegister(2, (int)ch);
				break;

			case SC_PutString:
				// notify a syscall for called functions
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					n = synchconsole->SynchPutString(buffer);
					// writes the number of characters written in return register
					machine->WriteRegister(2, n);
				}
				else
				{
					// copy error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_GetString:
				// notify a syscall for called functions
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				maxSize = machine->ReadRegister(5);
				dynBuffer = new char[maxSize];
				if(dynBuffer != NULL)
				{
					// allocation successfull
					n = synchconsole->SynchGetString(dynBuffer, maxSize);
					if(n == -1){
						machine->WriteRegister(2, -1);
					}else{
						if (copyStringToMachine(dynBuffer, adr))
						{
							// writes the number of characters read in return register
							machine->WriteRegister(2, n);
						}
						else
						{
							// copy error, writes -1 in return register
							machine->WriteRegister(2, -1);
						}
					}
					delete dynBuffer;
				}
				else
				{
					// allocation error : returns -1
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_PutInt:
				// read the number in argument
				n = machine->ReadRegister(4);
				// read the maximum of characters
				n = synchconsole->SynchPutInt(n);
				// writes the number of characters written in return register
				machine->WriteRegister(2, n);
				break;

	#ifdef step3
			case SC_UserThreadCreate:
				// notify a syscall for called functions
				currentThread->setIsSyscall(true);
				n = machine->ReadRegister(4);
				adr = machine->ReadRegister(5);
				codeErreur = do_UserThreadCreate(n, adr);
				machine->WriteRegister(2, codeErreur);
				break;

			case SC_UserThreadExit:
				// reads the return code of the thread
				codeErreur = machine->ReadRegister(4);
				do_UserThreadExit(codeErreur);
				break;

			case SC_GetTid:
				machine->WriteRegister(2, currentThread->tid);
				break;

			case SC_UserThreadJoin:
				n = machine->ReadRegister(4);
				adr = machine->ReadRegister(5);
				codeErreur = do_UserThreadJoin(n, adr);
				machine->WriteRegister(2, codeErreur);
				break;

			case SC_SemInit:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				n = machine->ReadRegister(5);
				isSuccess = do_SemInit(adr, n);

				if(isSuccess == -1)
					machine->WriteRegister(2, -1);
				else
					machine->WriteRegister(2, 0);
				break;

			case SC_SemWait:
				currentThread->setIsSyscall(true);
				n = machine->ReadRegister(4);
				isSuccess = machine->ReadMem(n, sizeof(int), &n);
				// error : returns -1
				if(!isSuccess || (do_SemWait(n) == -1))
					machine->WriteRegister(2, -1);
				else
					machine->WriteRegister(2, 0);
				break;

			case SC_SemPost:
				currentThread->setIsSyscall(true);
				n = machine->ReadRegister(4);
				isSuccess = machine->ReadMem(n, sizeof(int), &n);
				// error : returns -1
				if(!isSuccess || (do_SemPost(n) == -1))
					machine->WriteRegister(2, -1);
				else
					machine->WriteRegister(2, 0);
				break;

			case SC_SemDestroy:
				currentThread->setIsSyscall(true);
				n = machine->ReadRegister(4);

				// error : returns -1
				if(do_SemDestroy(n) == -1)
					machine->WriteRegister(2, -1);
				else
					machine->WriteRegister(2, 0);
				break;
#endif // STEP3

#ifdef step4
			case SC_ForkExec:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);

				// error : returns -1
				if ((n = do_forkExec(adr)) == -1)
					machine->WriteRegister(2, -1);
				else
					machine->WriteRegister(2, n);
				break;

			case SC_GetPid:
				machine->WriteRegister(2, currentProcess->getPid());
				break;

			case SC_WaitPid:
				n = machine->ReadRegister(4);
				if ((n = processManager->waitPid(n)) == -1)
					machine->WriteRegister(2, -1);
				else
					machine->WriteRegister(2, n);
				break;

			case SC_Mmap:
				n = machine->ReadRegister(4);
				n = do_mmap(n);
				machine->WriteRegister(2, n);
				break;

			case SC_Unmap:
				n = machine->ReadRegister(4);
				n = do_unmap(n);
				machine->WriteRegister(2, n);
				break;

			case SC_ArgStart:
				int m, o;
				m = machine->ReadRegister(5);
				n = machine->ReadRegister(6);
				o = machine->ReadRegister(7);
				n = do_arg_start(m, n, o);
				machine->WriteRegister(2, n);
				break;

			case SC_ArgArg:
				n = machine->ReadRegister(4);
				n = do_arg_arg(n);
				machine->WriteRegister(2, n);
				break;

			case SC_ArgEnd:
				n = machine->ReadRegister(4);
				n = do_arg_arg(n);
				machine->WriteRegister(2, n);
				break;

			case SC_GetNbProcessRunning:
				machine->WriteRegister(2,processManager->getNbProcessRunning());
				break;

			case SC_GetListProcess:
				n = machine->ReadRegister(4);
				machine->WriteRegister(2, processManager->getListProcess(n));
				break;

#endif // STEP4

#ifdef FILESYS
			case SC_Open:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					n = do_Open(buffer);
					// writes the number of characters written in return register
					machine->WriteRegister(2, n);
					semFileSys->V();
				}
				else
				{
					// copy error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Read:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				maxSize = machine->ReadRegister(5);
				n = machine->ReadRegister(6);
				semFileSys->P();
				n = do_Read(adr, maxSize, n);
				semFileSys->V();
				machine->WriteRegister(2, n);
				break;

			case SC_Write:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				maxSize = machine->ReadRegister(5);
				n = machine->ReadRegister(6);
				semFileSys->P();
				n = do_Write(adr, maxSize, n);
				semFileSys->V();
				machine->WriteRegister(2, n);
				break;

			case SC_Close:
				n = machine->ReadRegister(4);
				semFileSys->P();
				n = do_Close(n);
				semFileSys->V();
				machine->WriteRegister(2, n);
				break;


			case SC_Cd:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					n = fileSystem->cd(buffer);
					semFileSys->V();
					// writes the number of characters written in return register
					if (n) // if return is true
						machine->WriteRegister(2, 0);
					else
						machine->WriteRegister(2, -1);
				}
				else
				{
					// error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Pwd:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				semFileSys->P();
				dynBuffer = fileSystem->pwd();
				semFileSys->V();
				copyStringToMachine(dynBuffer, adr);
				delete dynBuffer;
				break;

			case SC_Mkdir:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					n = fileSystem->CreateDir(buffer);
					semFileSys->V();
					// writes the number of characters written in return register
					if (n) // if return is true
						machine->WriteRegister(2, 0);
					else
						machine->WriteRegister(2, -1);
				}
				else
				{
					// error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Ls:
				semFileSys->P();
				fileSystem->List();
				semFileSys->V();
				break;

			case SC_Rmdir:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					n = fileSystem->RemoveDirEmpty(buffer);
					semFileSys->V();
					// writes the number of characters written in return register
					if (n) // if return is true
						machine->WriteRegister(2, 0);
					else
						machine->WriteRegister(2, -1);
				}
				else
				{
					// error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_PathExists:
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					n = fileSystem->pathExist(buffer);
					semFileSys->V();
					// writes the number of characters written in return register
					if (n) // if return is true
						machine->WriteRegister(2, 1);
					else
						machine->WriteRegister(2, 0);
				}
				else
				{
					// error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Rm:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					n = fileSystem->RemoveFile(buffer);
					semFileSys->V();
					// writes the number of characters written in return register
					if (n) // if return is true
						machine->WriteRegister(2, 0);
					else
						machine->WriteRegister(2, -1);
				}
				else
				{
					// remove error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Create:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				n = machine->ReadRegister(5);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					semFileSys->P();
					isSuccess = fileSystem->Create(buffer, n);
					semFileSys->V();
				}
				else
				{
					isSuccess = false;
				}
				if(isSuccess)
				{
					// writes the number of characters written in return register
					machine->WriteRegister(2, 0);
				}
				else
				{
					// creation error, writes -1 in return register
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Copy:
				char buffer2[MAX_STRING_SIZE];
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					adr = machine->ReadRegister(5);
					if (copyStringFromMachine(adr, buffer2, MAX_STRING_SIZE-1))
					{
						semFileSys->P();
						Copy(buffer, buffer2);
						semFileSys->V();
					}
				}
				break;

			case SC_Print:
				currentThread->setIsSyscall(true);
				adr = machine->ReadRegister(4);
				// MAX_STRING_SIZE-1 to let space for the ‘\0’
				if (copyStringFromMachine(adr, buffer, MAX_STRING_SIZE-1))
				{
					Print(buffer);
					machine->WriteRegister(2, 0);
				}
				else
				{
					machine->WriteRegister(2, -1);
				}
				break;

			case SC_Random:
				n = machine->ReadRegister(4);
				RandomInit(time(NULL));
				n = Random() % n;
				// writes the number in return register
				machine->WriteRegister(2, n);
				break;

			case SC_Seek:
				n = machine->ReadRegister(4);
				adr = machine->ReadRegister(5);
				OpenFile *openfile;
				if (fm != NULL)
				{
					semFileSys->P();
					openfile = fm->getFile(adr);
					if(openfile != NULL)
					{
						openfile->Seek(n);
						machine->WriteRegister(2, 0);
					}
					else
					{
						machine->WriteRegister(2, -1);
					}
					semFileSys->V();
				}
				else
				{
					machine->WriteRegister(2, -1);
				}
				break;

#endif //FILESYS

#ifdef NETWORK
			case SC_InitSocket:
				n = machine->ReadRegister(4);		// Numero de la box
				// Renvoie l'ID de la socket creee ou -1
				machine->WriteRegister(2, do_SockInit(n));
				break;

			case SC_Send:
				n = machine->ReadRegister(4);		// Id de la socket
				adrBuffer=machine->ReadRegister(5); // Adresse du message
				// Renvoie le nombre de caractere envoyes ou -1
				machine->WriteRegister(2, currentProcess->socketManager->do_Send(n, adrBuffer));
				break;

			case SC_Receive:
				n = machine->ReadRegister(4);		// Id de la socket
				adrBuffer=machine->ReadRegister(5); // Adresse du message
				// Renvoie le nombre de caractere recuperes ou -1
				machine->WriteRegister(2, currentProcess->socketManager->do_Receive(n, adrBuffer));
				break;

			case SC_Sleep:
				n = machine->ReadRegister(4);
				Delay(n);
				break;

			case SC_GetHostname:
				machine->WriteRegister(2,postOffice->getNetAddr());
				break;
#endif // NETWORK
			case SC_Exit:
				// read return code in r4 register
				codeErreur = machine->ReadRegister(4);
				do_exit(codeErreur);
				break;

			default:
				printf("Unknown syscall\n");
				currentProcess->killProcess();
		}
		currentThread->setIsSyscall(false);
		// LB: Do not forget to increment the pc before returning!
		UpdatePC ();
	}
	else if ((which == AddressErrorException || which == PageFaultException) && currentThread->getIsSyscall()) {
		// we do nothing, exception is ignored and current syscall will return -1
	}
#else
	if ((which == SyscallException) && (type == SC_Halt))
	{
		DEBUG ('a', "Shutdown, initiated by user program.\n");
		interrupt->Halt ();
	}
#endif // CHANGED
	else
	{
				// Print the exception name for practical purpose
#ifdef CHANGED
		switch (which)
		{
#ifdef step4
			case PageFaultException:
				interrupt->SetLevel (IntOff);
				// address of the user's stack pointer
				adr = machine->ReadRegister(StackReg);
				// if is in stack => Stack overflow
				if (currentProcess->getAddrSpace()->addrSpaceAllocator->isInStack(adr))
				{
					Printf("STACK OVERFLOW !!\n");
				}
				else
				{
					Printf("PageFaultException\n");
					ASSERT(false);
				}
				break;
#else
			Printf ("Unexpected user mode exception (%d:", which);
			case PageFaultException:	Printf("PageFaultException\n");		break;
			ASSERT(FALSE);
#endif // step4

			Printf ("Unexpected user mode exception (%d:", which);
			case SyscallException: 		Printf("SyscallException \n"); 		break;
			case ReadOnlyException: 	Printf("ReadOnlyException \n"); 		break;
			case BusErrorException: 	Printf("BusErrorException \n"); 		break;
			case AddressErrorException: Printf("AddressErrorException \n"); 	break;
			case OverflowException: 	Printf("OverflowException \n"); 		break;
			case IllegalInstrException: Printf("IllegalInstrException \n"); 	break;
			default: 					Printf("Unknown "); 				break;
		}
#endif //CHANGED

#ifdef CHANGED
#ifdef step4
		currentProcess->killProcess();
#else
		Printf("type:%d)\n", type);
		ASSERT(FALSE);
#endif
#endif //CHANGED

	} // fin else
#ifndef CHANGED
	UpdatePC ();
#endif
}

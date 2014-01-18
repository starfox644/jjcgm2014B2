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
#include "synchconsole.h"

extern int do_UserThreadCreate(int f, int arg);
extern int do_UserThreadJoin(int tid, int addrUser);
extern void do_UserThreadExit(int status);
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

			case SC_GetNbProcess:
				machine->WriteRegister(2,processManager->getNbProcess());
				break;

			case SC_GetListProcess:
				n = machine->ReadRegister(4);
				machine->WriteRegister(2, processManager->getListProcess(n));
				break;

#endif // STEP4

			case SC_Exit:
				// read return code in r4 register
				codeErreur = machine->ReadRegister(4);
				do_exit(codeErreur);
				break;


			default: {
#ifdef CHANGED
				Printf ("Unexpected user mode exception (%d:", which);
				// Print the exception name for practical purpose
				switch (which) {
				case SyscallException: 		Printf("SyscallException "); 		break;
				case PageFaultException:	Printf("PageFaultException ");		break;
				case ReadOnlyException: 	Printf("ReadOnlyException "); 		break;
				case BusErrorException: 	Printf("BusErrorException "); 		break;
				case AddressErrorException: Printf("AddressErrorException "); 	break;
				case OverflowException: 	Printf("OverflowException "); 		break;
				case IllegalInstrException: Printf("IllegalInstrException "); 	break;
				default: 					Printf("Unknown "); 				break;
				}
				Printf("type:%d)\n", type);
				ASSERT (FALSE);
#endif	//CHANGED
			}
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
		switch (which) {
		#ifdef step4
				case PageFaultException:
					// address of the user's stack pointer
					adr = machine->ReadRegister(StackReg);
					// if is in stack => Stzck overflow
					if (currentProcess->getAddrSpace()->addrSpaceAllocator->isInStack(adr))
					{
						//#ifdef CHANGED
						Printf("STACK OVERFLOW !!\n");
						//#endif // CHANGED
					}
					else
					{
						//#ifdef CHANGED
						Printf("PageFaultException ");
						//#endif // CHANGED
					}
					break;
		#else
				//#ifdef CHANGED
				Printf ("Unexpected user mode exception (%d:", which);
				case PageFaultException:	Printf("PageFaultException ");		break;
				//#endif //CHANGED
		#endif // step4
//#ifdef CHANGED
				Printf ("Unexpected user mode exception (%d:", which);
				case SyscallException: 		Printf("SyscallException "); 		break;
				case ReadOnlyException: 	Printf("ReadOnlyException "); 		break;
				case BusErrorException: 	Printf("BusErrorException "); 		break;
				case AddressErrorException: Printf("AddressErrorException "); 	break;
				case OverflowException: 	Printf("OverflowException "); 		break;
				case IllegalInstrException: Printf("IllegalInstrException "); 	break;
				default: 					Printf("Unknown "); 				break;
			}
#endif //CHANGED
#ifndef step4
#ifdef CHANGED
				Printf("type:%d)\n", type);
				currentProcess->killProcess();
#endif //CHANGED
#endif

			}
			// End of addition
		#ifndef CHANGED
			UpdatePC ();
		#endif
}

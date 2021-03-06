/* Start.s 
 *	Assembly language assist for user programs running on top of Nachos.
 *
 *	Since we don't want to pull in the entire C library, we define
 *	what we need for a user program here, namely Start and the system
 *	calls.
 */
#include "syscall.h"

        .text   
        .align  2

/* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main". 
 *
 * 	NOTE: This has to be first, so that it gets loaded at location 0.
 *	The Nachos kernel always starts a program by jumping to location 0.
 * -------------------------------------------------------------
 */

	.globl __start
	.ent	__start
__start:
	jal	main
#ifdef CHANGED
	// la valeur de retour est dans r2, on la met dans r4 pour appeler exit avec
	move	$4,$2
#else
	move	$4,$0		
#endif
	jal	Exit	 /* if we return from main, exit(0) */
	.end __start

#ifdef CHANGED
	.globl __startThread
		.ent __startThread
	__startThread:
		move $4,$27				// copy of the arg
		jal	$26					// jump to the thread's function
		move $4,$0
		jal	UserThreadExit		// if we return from the function, call UserThreadExit(0) */
		.end __startThread
#endif

/* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is 
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 * 	The return value is in r2. This follows the standard C calling
 * 	convention on the MIPS.
 * -------------------------------------------------------------
 */
/* ********************************* */
	.globl Halt
	.ent	Halt
Halt:
	addiu $2,$0,SC_Halt
	syscall
	j	$31
	.end Halt
/* ********************************* */
	.globl Exit
	.ent	Exit
Exit:
	addiu $2,$0,SC_Exit
	syscall
	j	$31
	.end Exit
/* ********************************* */
	.globl Exec
	.ent	Exec
Exec:
	addiu $2,$0,SC_Exec
	syscall
	j	$31
	.end Exec
/* ********************************* */
	.globl Join
	.ent	Join
Join:
	addiu $2,$0,SC_Join
	syscall
	j	$31
	.end Join
/* ********************************* */
	.globl Create
	.ent	Create
Create:
	addiu $2,$0,SC_Create
	syscall
	j	$31
	.end Create
/* ********************************* */
	.globl Open
	.ent	Open
Open:
	addiu $2,$0,SC_Open
	syscall
	j	$31
	.end Open
/* ********************************* */
	.globl Read
	.ent	Read
Read:
	addiu $2,$0,SC_Read
	syscall
	j	$31
	.end Read
/* ********************************* */
	.globl Write
	.ent	Write
Write:
	addiu $2,$0,SC_Write
	syscall
	j	$31
	.end Write
/* ********************************* */
	.globl Close
	.ent	Close
Close:
	addiu $2,$0,SC_Close
	syscall
	j	$31
	.end Close
/* ********************************* */
	.globl Fork
	.ent	Fork
Fork:
	addiu $2,$0,SC_Fork
	syscall
	j	$31
	.end Fork
/* ********************************* */
	.globl Yield
	.ent	Yield
Yield:
	addiu $2,$0,SC_Yield
	syscall
	j	$31
	.end Yield

#ifdef CHANGED
/* ********************************* */
	.globl PutChar
	.ent	PutChar
PutChar:
	addiu $2,$0,SC_PutChar
	syscall
	j	$31
	.end PutChar
/* ********************************* */
	.globl PutString
	.ent	PutString
PutString:
	addiu $2,$0,SC_PutString
	syscall
	j	$31
	.end PutString
/* ********************************* */
	.globl PutInt
	.ent	PutInt
PutInt:
	addiu $2,$0,SC_PutInt
	syscall
	j	$31
	.end PutInt
/* ********************************* */
	.globl GetChar
	.ent	GetChar
GetChar:
	addiu $2,$0,SC_GetChar
	syscall
	j	$31
	.end GetChar
/* ********************************* */
	.globl GetString
	.ent	GetString
GetString:
	addiu $2,$0,SC_GetString
	syscall
	j	$31
	.end GetString
/* ********************************* */
	.globl UserThreadCreate
	.ent	UserThreadCreate
UserThreadCreate:
	addiu $2,$0,SC_UserThreadCreate
	syscall
	j	$31
	.end UserThreadCreate
/* ********************************* */
.globl UserThreadExit
	.ent	UserThreadExit
UserThreadExit:
	addiu $2,$0,SC_UserThreadExit
	syscall
	j	$31
	.end UserThreadExit
/* ********************************* */
.globl SemInit
	.ent	SemInit
SemInit:
	addiu $2,$0,SC_SemInit
	syscall
	j	$31
	.end SemInit
/* ********************************* */
.globl SemWait
	.ent	SemWait
SemWait:
	addiu $2,$0,SC_SemWait
	syscall
	j	$31
	.end SemWait
/* ********************************* */
.globl SemPost
	.ent	SemPost
SemPost:
	addiu $2,$0,SC_SemPost
	syscall
	j	$31
	.end SemPost
/* ********************************* */
.globl SemDestroy
	.ent	SemDestroy
SemDestroy:
	addiu $2,$0,SC_SemDestroy
	syscall
	j	$31
	.end SemDestroy
/* ********************************* */
.globl GetTid
	.ent	GetTid
GetTid:
	addiu $2,$0,SC_GetTid
	syscall
	j	$31
	.end GetTid
/* ********************************* */
	.globl UserThreadJoin
	.ent	UserThreadJoin
UserThreadJoin:
	addiu $2,$0,SC_UserThreadJoin
	syscall
	j	$31
	.end UserThreadJoin
/* ********************************* */
	.globl ForkExec
	.ent	ForkExec
ForkExec:
	addiu $2,$0,SC_ForkExec
	syscall
	j	$31
	.end ForkExec
/* ********************************* */
	.globl GetPid
	.ent	GetPid
GetPid:
	addiu $2,$0,SC_GetPid
	syscall
	j	$31
	.end GetPid
/* ********************************* */
	.globl WaitPid
	.ent	WaitPid
WaitPid:
	addiu $2,$0,SC_WaitPid
	syscall
	j	$31
	.end WaitPid
/* ********************************* */
	.globl Mmap
	.ent	Mmap
Mmap:
	addiu $2,$0,SC_Mmap
	syscall
	j	$31
	.end Mmap
/* ********************************* */
	.globl Unmap
	.ent	Unmap
Unmap:
	addiu $2,$0,SC_Unmap
	syscall
	j	$31
	.end Unmap
/* ********************************* */
	.globl GetListProcess
	.ent	GetListProcess
GetListProcess:
	addiu $2,$0,SC_GetListProcess
	syscall
	j	$31
	.end GetListProcess
/* ********************************* */
	.globl arg_start
	.ent	arg_start
arg_start:
	addiu $2,$0,SC_ArgStart
	syscall
	j	$31
	.end arg_start
/* ********************************* */
	.globl arg_arg
	.ent	arg_arg
arg_arg:
	addiu $2,$0,SC_ArgArg
	syscall
	j	$31
	.end arg_arg
/* ********************************* */
	.globl arg_end
	.ent	arg_end
arg_end:
	addiu $2,$0,SC_ArgEnd
	syscall
	j	$31
	.end arg_end
/* ********************************* */
	.globl GetNbProcessTotal
	.ent	GetNbProcessTotal
GetNbProcessTotal:
	addiu $2,$0,SC_GetNbProcessTotal
	syscall
	j	$31
	.end GetNbProcessTotal
/* ********************************* */
	.globl cd
	.ent	cd
cd:
	addiu $2,$0,SC_Cd
	syscall
	j	$31
	.end cd
/* ********************************* */
	.globl pwd
	.ent	pwd
pwd:
	addiu $2,$0,SC_Pwd
	syscall
	j	$31
	.end pwd
/* ********************************* */
	.globl mkdir
	.ent	mkdir
mkdir:
	addiu $2,$0,SC_Mkdir
	syscall
	j	$31
	.end mkdir
/* ********************************* */
	.globl ls
	.ent	ls
ls:
	addiu $2,$0,SC_Ls
	syscall
	j	$31
	.end ls
/* ********************************* */
	.globl rmdir
	.ent	rmdir
rmdir:
	addiu $2,$0,SC_Rmdir
	syscall
	j	$31
	.end rmdir
/* ********************************* */
	.globl pathExists
	.ent	pathExists
pathExists:
	addiu $2,$0,SC_PathExists
	syscall
	j	$31
	.end pathExists
/* ********************************* */
	.globl rm
	.ent	rm
rm:
	addiu $2,$0,SC_Rm
	syscall
	j	$31
	.end rm
/* ********************************* */
	.globl copy
	.ent	copy
copy:
	addiu $2,$0,SC_Copy
	syscall
	j	$31
	.end copy
/* ********************************* */
	.globl Cat
	.ent	Cat
Cat:
	addiu $2,$0,SC_Print
	syscall
	j	$31
	.end Cat
/* ********************************* */
	.globl Random
	.ent	Random
Random:
	addiu $2,$0,SC_Random
	syscall
	j	$31
	.end Random
/* ********************************* */
	.globl Seek
	.ent	Seek
Seek:
	addiu $2,$0,SC_Seek
	syscall
	j	$31
	.end Seek
/* ********************************* */
	.globl InitSocket
	.ent	InitSocket
InitSocket:
	addiu $2,$0,SC_InitSocket
	syscall
	j	$31
	.end InitSocket
/* ********************************* */
	.globl Send
	.ent	Send
Send:
	addiu $2,$0,SC_Send
	syscall
	j	$31
	.end Send
/* ********************************* */
	.globl Receive
	.ent	Receive
Receive:
	addiu $2,$0,SC_Receive
	syscall
	j	$31
	.end Receive
/* ********************************* */
.globl Sleep
	.ent	Sleep
Sleep:
	addiu $2,$0,SC_Sleep
	syscall
	j	$31
	.end Sleep
/* ********************************* */
.globl GetHostname
	.ent	GetHostname
GetHostname:
	addiu $2,$0,SC_GetHostname
	syscall
	j	$31
	.end GetHostname
/* ********************************* */
#endif /* CHANGED */

/* dummy function to keep gcc happy */
        .globl  __main
        .ent    __main
__main:
        j       $31
        .end    __main


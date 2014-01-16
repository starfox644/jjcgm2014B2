#ifdef CHANGED
#ifndef ___THREADMANAGER
#define ___THREADMANAGER
class Thread;
#include <list>

class ThreadManager
{
  public:

	ThreadManager ();

	~ThreadManager ();

	/**
	 *  add a thread to this address space
	 */
	void addThread(Thread *th);

	/**
	 *  remove a thread from this address space
	 */
	void removeThread(Thread *th);

	/*
	 * 	returns the number of user threads, without the main thread
	 */
	int getNbThreads();

	/*
	 * returns the thread with this tid.
	 * If isn't in l_threads, return NULL
	 */
	Thread* searchThread(int tid);
	/**
	 * delete list of threads
	 */
	void deleteThreads();

	// threads of the address space
	std::list<Thread*> l_threads;

	// number of threads in execution without the main thread
	int nbThreads;
};

#endif // ___THREADMANAGER
#endif // CHANGED

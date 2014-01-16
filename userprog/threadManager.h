#ifdef CHANGED
#ifndef ___THREADMANAGER
#define ___THREADMANAGER

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

	/**
	 * 	returns the number of user threads, without the main thread
	 */
	int getNbThreads();

	// threads of the address space
	std::list<Thread*> l_threads;

	// number of threads in execution without the main thread
	int nbThreads;

	/**
	 * delete list of threads
	 */
	void deleteThreads();

//  private:

	// number of threads in execution without the main thread
	//int nbThreads;


	// number max of threads depending on memory for the stacks
	//int maxThreads;

	/**
	 * delete list of threads
	 */
	//void deleteThreads();
};

#endif // ___THREADMANAGER
#endif // CHANGED

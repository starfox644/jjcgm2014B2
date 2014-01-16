#ifdef CHANGED
#include "threadManager.h"
#include "copyright.h"
#include "system.h"


ThreadManager::ThreadManager ()
{
	nbThreads = 0;
}


ThreadManager::~ThreadManager ()
{
	nbThreads = 0;
}
void ThreadManager::addThread(Thread *th)
{
	nbThreads++;
	// add the new thread in threads list
	l_threads.push_back(th);
}

void ThreadManager::removeThread(Thread *th)
{
	nbThreads--;
}

int ThreadManager::getNbThreads()
{
	return nbThreads;
}

void ThreadManager::deleteThreads()
{
	std::list<Thread*>::iterator it;
	for(it = l_threads.begin() ; it != l_threads.end() ; it++)
	{
		delete *it;
	}
}


#endif


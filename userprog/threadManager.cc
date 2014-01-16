#ifdef CHANGED
#include "threadManager.h"
#include "copyright.h"
#include "system.h"
#include "thread.h"

ThreadManager::ThreadManager ()
{
	nbThreads = 0;
	s_nbThreads = new Semaphore("nbThread semaphore", 1);
	s_userJoin = new Semaphore("user join semaphore", 1);
}

ThreadManager::~ThreadManager ()
{
	delete s_nbThreads;
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

Thread* ThreadManager::searchThread(int tid)
{
	std::list<Thread*>::iterator it = currentProcess->threadManager->l_threads.begin();
	while (it != currentProcess->threadManager->l_threads.end() && (tid != (*it)->tid))
	{
		++it;
	}
	if (it == currentProcess->threadManager->l_threads.end())
		return NULL;
	else
		return *it;
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


/*
 * semaphoreManager.h
 *
 *  Created on: 16 janv. 2014
 *      Author: Sarkal
 */
#ifdef CHANGED
#ifndef __SEMAPHOREMANAGER__H__
#define __SEMAPHOREMANAGER__H__

#include "synch.h"

class SemaphoreManager {

public :
	SemaphoreManager();
	~SemaphoreManager();

	// Semaphore list : needed to give an unique identifier for user semaphores
	std::list<Semaphore*> semList;
	/**
	 * Add newSem to semList and allocate it a unique modifier
	 */
	int addSemaphore(int initValue);

	/**
	 * Remove a semaphore from the list based on his identifier.
	 * If the identifier is valid, the semaphore is destroyed.
	 * If not, the function returns -1.
	 */
	int removeSemaphore(int id);

	/**
	 * Return the semaphore identified by id, or NULL if it doesn't exist
	 */
	Semaphore* getSemaphore(int id);

	/**
	 * Delete the semaphore list
	 */
	void deleteSemaphores();

private :
	// number of semaphore created
	int nbSem;
	int nextSemId;
};
#endif // __SEMAPHOREMANAGER__H__
#endif // CHANGED






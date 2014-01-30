#ifdef CHANGED
/*
 * semaphoreManager.cc
 * Gere la liste des semaphores d'un processus
 *  Created on: 16 janv. 2014
 *      Author: Sarkal
 */

#include "semaphoreManager.h"
#include <iostream>

#ifdef countNew
#include "countNew.h"
#endif



SemaphoreManager::SemaphoreManager (){
	nbSem = 0;
	nextSemId = 0;
}

SemaphoreManager::~SemaphoreManager (){
	deleteSemaphores();
}

/**
 * Add a new semaphore with initValue and give it an unique id.
 * Return -1 if nextSemId has reached MAX_INT
 */
int SemaphoreManager::addSemaphore(int initValue)
{
	// unique name based on identifier
	if (nextSemId < INT_MAX)
	{
		const char *name = "UserSem" + nextSemId;
		Semaphore *sem = new Semaphore (name, initValue);
		// Set the semaphore id
		sem->setId(nextSemId);
		nbSem++;
		nextSemId++;
		// Add it to the list
		semList.push_back(sem);
		return sem->getId();
	}
	else
		return -1;
}

/**
 * Remove a semaphore from the list based on his identifier.
 * If the identifier is valid, the semaphore is destroyed.
 * If not, the function returns -1.
 */
int SemaphoreManager::removeSemaphore(int id)
{
	// iterator to find the semaphore in the list
	std::list<Semaphore*>::iterator it=semList.begin();
	while (it != semList.end() && (*it)->id != id)
		it++;
	// If semaphore not found, return -1 : error
	if ((*it)->id != id)
		return -1;
	// Else, remove it
	else
	{
		nbSem--;
		delete *it;
		semList.erase(it);
		return 0;
	}
}

/**
 * Return the semaphore identified by id, or NULL if it doesn't exist
 */
Semaphore* SemaphoreManager::getSemaphore(int id)
{
	// iterator to find the semaphore in the list
	std::list<Semaphore*>::iterator it=semList.begin();
	while (it != semList.end() && (*it)->id != id)
		it++;
	if(it == semList.end())
		return NULL;
	// If semaphore not found, return -1 : error
	if ((*it)->id != id)
		return NULL;
	// Else, return it
	else
		return (Semaphore*)(*it);
}

/**
 * Delete the semaphore list
 */
void SemaphoreManager::deleteSemaphores()
{
	std::list<Semaphore*>::iterator it;
	for (it = semList.begin() ; it != semList.end() ; it++)
		delete *it;
}

#endif // CHANGED




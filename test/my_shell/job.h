/*
 * job.h
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef JOB_H_
#define JOB_H_

#define MAX_CHAR 128
#define MAX_JOB 1	//nombre maximum de jobs possible
#define RUN 1		//etat d'un processus lors de l'affichage
#define STOP 0		//etat d'un processus lors de l'affichage

typedef struct job{
	int numero;
	int etat;
	char cmdLine[MAX_CHAR];

}job;

void afficherJobs();

#endif /* JOB_H_ */

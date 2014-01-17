/*
 * job.h
 *
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef JOB_H_
#define JOB_H_

#include "eval.h"

#define MAX_CHAR 128
#define MAX_JOB 1	//nombre maximum de jobs possible
#define RUN 1		//etat d'un processus lors de l'affichage
#define STOP 0		//etat d'un processus lors de l'affichage
#define BACK 1		//si le processus est en arriere plan
#define FIRST 0		//si le processus est au premier plan


typedef struct job{
	int numero;				//numéro du job prend le premier libre dans notre tableau qui est limite a MAXJOBS qui est un choix arbitraire du au projet de SR de L3(peux etre changé si on veut)
	int pid;
	int etat;				// etat du job en cours ou arreter
	int plan;				//plan du job au premier ou arriere plan
	char command[MAX_CHAR];	//ligne de commande exécuté par le job

}job;


/*
 * initialisation du tableau des jobs
 */
void init_jobs();

/*
 * fonction renvoyant le job en premier plan, -1 si il n'y en a pas
 */
int get_premier_jobs();

/*
 *fonction qui renvoit la première case du tableau libre(ce qui correspond au premier  numéro de job libre)
 */
int premier_libre();
/*
 * Fonction de création d'un job avec en paramètre le pid
 * l'état sera en run , le plan dépendra de l'option '&'
 * et la ligne de commande entré par l'utilisateur
 */
void add_job(int pid, int plan,char* commande);

/*fonction de suppresion d'un jobs
 * au choix soit par son numéro de jobs
 * soit par son pid
 */
void supp_job(int num);
void supp_job_pid(int pid);

/*
 * fonction de mise en premier plan du job
 */
void fg(char** argv);
/*
 * fonction de mise en arriere plan du job
 */
void bg(char** argv);

/*
 * affichage des jobs en cours
 */
void afficherJobs();

#endif /* JOB_H_ */

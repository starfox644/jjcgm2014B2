/*
 * job.c
 *	Ici nous allons créer une structure pour représenté les processus tournant dans le shell
 *	Nous allons les afficher dans le shell est les créer
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#include "job.h"
//declaration de notre tableau des jobs
job tab_Job[MAX_JOB + 1];

/*
 * initialisation du tableau des jobs
 */
void init_jobs(){
	int i;
	for(i = 0; i < MAX_JOB + 1; i++){
		tab_job[i].numero = -1;
		tab_job[i].etat = STOP;
		tab_job[i].plan = BACK;
	}
}

int get_premier_jobs(){
	int i = 0;
	while(i < MAX_JOB && tab_job[i].plan != FIRST){ //tantque i est inférieur a MAX_JOB et tant qu'on ne trouve pas un job au premier plan on incrémente
		i++;
	}
	if(tab_job[i].plan == first){// si celui qu'on a trouver est bien au premier plan alors on renvoi son numéro
		return tab_job[i].numero;
	}else{	//sinon erreur on retourne -1
		return -1;
	}

}

int premier_libre(){
	int libre = 0;
	while(tab_job[libre].numero != -1){
		libre++;
	}
	if(tab_job[libre].numero == -1){
		return libre;
	}else{
		return -1;
	}
}

void add_job(int pid, int plan,char* commande){
	int numero_job = premier_libre();
	tab_job[numero_job].numero = numero_job;
	tab_job[numero_job].pid = pid;
	tab_job[numero_job].etat = RUN;
	tab_job[numero_job].plan = plan;
	StrCpy(tab_job[numero_job].commande,commande);

}

void supp_job(int num){
	tab_job[i].numero = -1;
	tab_job[i].etat = STOP;
	tab_job[i].plan = BACK;

}
void supp_job_pid(int pid){
	int i = 0;
	while(tab_job[i].pid != pid){
		i++;
	}
	if(tab_job[i].pid == pid){
		tab_job[i].numero = -1;
		tab_job[i].etat = STOP;
		tab_job[i].plan = BACK;

	}
}

/*
 * fonction de mise en premier plan du job
 */
void fg(char** argv);
/*
 * fonction de mise en arriere plan du job
 */
void bg(char** argv);


void afficherJobs(){
	int i = 0;
	while(i < MAX_JOB){
		if(tab_job[i].numero != -1){
			Printf("Job en cours :\n",0);
			Printf("job\t\tpid\\etat\t\tplan\n",0);
		}
	}

}

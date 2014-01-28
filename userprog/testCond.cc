/*
 * testCond.cc
 *
 *  Created on: 21 janv. 2014
 *      Author: galdween
 */
#ifdef CHANGED
#include "system.h"
#include <stdio.h>
#include "copyright.h"
#include "thread.h"
#include "synch.h"
#include "utility.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"
#include "lecteur_redacteur.h"



struct donnees_thread{
	lecteur_redacteur_t lecteur_redacteur;
	int iterations;
	int donnee;
};

typedef struct donnees_thread donnees_thread_t;

void dodo(int scale){
	Delay(Random()%1000000 * scale);
}

void lecteur (int args){
	printf("lecteur\n");
	donnees_thread_t *d = (donnees_thread_t*)args;
	int i, valeur;
	for(i = 0; i< d->iterations; i++){
		dodo(2);
		debut_lecture(&d->lecteur_redacteur);
		valeur = d->donnee;
		dodo(1);
		if(valeur!= d->donnee){
			printf("Lecture incoherente!!\n");
		}else{
			printf("lecture cohérente\n");
		}
		fin_lecture(&d->lecteur_redacteur);
	}
}

void redacteur(int args){
	printf("redacteur\n");
	donnees_thread_t *d = (donnees_thread_t*) args;
	int i,valeur;

	for(i = 0;i < d->iterations;i++){
		dodo(2);
		debut_redaction(&d->lecteur_redacteur);

		valeur = Random();
		d->donnee = valeur;
		dodo(1);
		if(valeur != d->donnee){
			printf("Redaction incoherente!!\n");
		}else{
			printf("rédaction cohérente\n");
		}
		fin_redaction(&d->lecteur_redacteur);
	}
}

int testCond(){
	printf("testCond?\n");
	Thread *threads,*thread_courant;
	donnees_thread_t *donnees_thread = new donnees_thread_t;
	int i, nb_lecteurs, nb_redacteurs;

	nb_lecteurs = 4;
	nb_redacteurs = 2;

	donnees_thread->iterations = 2;

	threads = new Thread("thread thread");
	thread_courant = threads;
	initialiser_lecteur_redacteur(&donnees_thread->lecteur_redacteur);

	printf("debut boucle creation\n");
	for(i = 0; i < nb_lecteurs; i++){
		thread_courant->Fork(lecteur,(int)donnees_thread);
	}
	for(i = 0; i < nb_redacteurs; i++){
		thread_courant->Fork(redacteur,(int)donnees_thread);
	}
	//for(i = 0; i < nb_redacteurs+nb_lecteurs;i++){
		//thread_courant[i].Finish();
	//}
	delete threads;
	return 0;
}
#endif


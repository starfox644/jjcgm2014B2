/*
 * testcond.h
 *
 *  Created on: 21 janv. 2014
 *      Author: galdween
 */
#ifdef CHANGED
#ifndef TESTCOND_H_
#define TESTCOND_H_
#include "synch.h"

typedef struct lecteur_redacteur lecteur_redacteur_t;
struct lecteur_redacteur{
		int nb_Writer;
		int nb_Reader;
		Semaphore *fifo;
		Lock *verrou;
		Condition *cond_Lecteur;
		Condition *cond_ecriture;

};

void initialiser_lecteur_redacteur(lecteur_redacteur_t *lecteur_redacteur);
void debut_lecture(lecteur_redacteur_t *lecteur_redacteur);
void fin_lecture(lecteur_redacteur_t *lecteur_redacteur);
void debut_redaction(lecteur_redacteur_t *lecteur_redacteur);
void fin_redaction(lecteur_redacteur_t *lecteur_redacteur);
void detruire_lecteur_redacteur(lecteur_redacteur_t *lecteur_redacteur);





#endif /* TESTCOND_H_ */
#endif //CHANGED

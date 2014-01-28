//fichier lecteur_redacteur.c  pour Exercice 2 TD-TP 3 CSE
// auteur : Michael OMER / Heda WANG
#ifdef CHANGED
#include "lecteur_redacteur.h"

//on initialise le lecteur redacteur
void initialiser_lecteur_redacteur(lecteur_redacteur_t *lecteur_redacteur){

	// on initialise les differentes informations de la structure
	lecteur_redacteur->nb_Writer = 0;
	lecteur_redacteur->nb_Reader = 0;
	lecteur_redacteur->verrou = new Lock("lock verrou");
	lecteur_redacteur->cond_Lecteur = new Condition("condition lecteur");
	lecteur_redacteur->cond_ecriture = new Condition("condition ecriture");

}

//fonction pour la lecture
void debut_lecture(lecteur_redacteur_t *lecteur_redacteur){
	printf("debut lecture\n");

	lecteur_redacteur->verrou->Acquire();

	while(lecteur_redacteur->nb_Writer > 0 ){
		lecteur_redacteur->cond_Lecteur->Wait(lecteur_redacteur->verrou);
	}

	lecteur_redacteur->nb_Reader++;
	lecteur_redacteur->verrou->Release();

}

void fin_lecture(lecteur_redacteur_t *lecteur_redacteur){
	printf("fin lecture\n");
	lecteur_redacteur->verrou->Acquire();
	lecteur_redacteur->nb_Reader--;
	if(lecteur_redacteur->nb_Reader == 0){
		lecteur_redacteur->cond_ecriture->Signal(lecteur_redacteur->verrou);
	}
	lecteur_redacteur->verrou->Release();
}

//fonction pour l'ecriture
void debut_redaction(lecteur_redacteur_t *lecteur_redacteur){
	printf("debut redaction\n");

	lecteur_redacteur->verrou->Acquire();
	while(lecteur_redacteur->nb_Reader > 0 ||lecteur_redacteur->nb_Writer > 0 ){
		lecteur_redacteur->cond_ecriture->Wait(lecteur_redacteur->verrou);
	}
	lecteur_redacteur->nb_Writer++;
	lecteur_redacteur->verrou->Release();
}

void fin_redaction(lecteur_redacteur_t *lecteur_redacteur){
	printf("fin redaction\n");
	//on verrouille le redacteur
	lecteur_redacteur->verrou->Acquire();
	//on decremente le nombre d'ecrivain
	lecteur_redacteur->nb_Writer--;
	//on reveille tout le monde
	lecteur_redacteur->cond_Lecteur->Broadcast(lecteur_redacteur->verrou);
	lecteur_redacteur->cond_ecriture->Signal(lecteur_redacteur->verrou);
	//on lache notre verrou
	lecteur_redacteur->verrou->Release();

}

void detruire_lecteur_redacteur(lecteur_redacteur_t *lecteur_redacteur){
	delete lecteur_redacteur->verrou;
	delete lecteur_redacteur->cond_Lecteur;
	delete lecteur_redacteur->cond_ecriture;
}
#endif

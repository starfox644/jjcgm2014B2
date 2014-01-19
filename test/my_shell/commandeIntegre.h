/*
 * commandIntegre.h
 *
 *  Created on: 19 janv. 2014
 *      Author: galdween
 */

#ifndef COMMANDINTEGRE_H_
#define COMMANDINTEGRE_H_

#include "syscall.h"
#include "../nachos_libc/String.h"
#include "../nachos_libc/Printf.h"
#include "../nachos_libc/File.h"
#include "../nachos_libc/util.h"

/*
 * fonction permettant d'afficher les processus système en cours
 */
void afficherProcess();

/*
 * fonction permettant d'afficher le contenu du dossier courant
 */
void do_ls();

/*
 * Fonction permettant d'afficher le chemin local
 */
void do_pwd();

/*
 * fonction permettant de copier un fichier Unix vers un fichier NachOS
 */
void do_copie(char*, char*);

/*
 * fonction permettant de supprimer le fichier passé en paramètre du système NachOS
 */
void do_remove(char*);
/*
 * fonction permettant d'afficher l'intégralité du contenu du systeme de fichier
 */
void afficherSysteme();
/*
 * fonction permettant de naviguer dans le système de fichier
 */
void do_cd(char*);

/*
 * fonction permettant d'afficher le manuel d'aide des commandes
 */
void afficherManuel(char*);

/*
 * fonction permettant de gerer le reseau
 */
void do_inet(char*);

#endif /* COMMANDINTEGRE_H_ */

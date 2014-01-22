/*
 * File.h
 *	fichier librairies pour utilisateur concernant la gestion des fichiers sous NachOS
 *  Created on: 17 janv. 2014
 *      Author: galdween
 */

#ifndef FILE_H_
#define FILE_H_
//definition des flags
#define O_RDONLY (0)	// mode d'ouverture en lecture seule
#define O_WRONLY (1)	//mode d'ouverture en ecriture seule
#define O_RDWR (2)		//mode d'ouverture en lecture/ecriture
//definition des modes
#define O_APPEND (10) // mode d'ouverture du fichier avec l'offset a la fin du fichier
#define O_CREAT (11)	//mode de creation d'un fichier

//definition pour le positionnement de l'offset
#define SEEK_SET (20)	//a partir du début du fichier
#define SEEK_CUR (21)	//a partir de la position ou on ce trouve
#define SEEK_END (22)	//a partir de la fin du fichier

/*
 * Fonction permettant d'ouvrir un fichier
 * suivant le flag choisi on l'ouvre en ecriture ou lecture
 * suivant le mode on crée le fichier ou on ce position a la fin
 * Renvoi le file Descripteur
 */
int OpenFile(char* cheminDuFichier, int flags, int mode);
/*
 * Fonction permettant de lire une partie du fichier
 * On lui donne le File descripteur donner par openFile
 * Permet de lire le nombre de caractère donnée par taille
 */
int ReadFile(int fileDescripteur, int taille);
/*
 * Fonction permettant d'écrire une certaine taille de donnée dans un fichier
 *
 */
int WriteFile(int fileDescripteur, void* buffer, int taille);
/*
 * Fonction permettant de ce positionner dans un fichier
 * position est un mode defini au dessus SEEK_SET / SEEK_CUR/ SEEK_END
 */
int SeekFile(int fileDescripteur, int offset,int position);
/*
 * Fonction permettant de fermer un fichier et de libérer la mémoire
 */
int CloseFile(int fileDescripteur);

#endif /* FILE_H_ */

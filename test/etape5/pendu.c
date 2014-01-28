/*#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>*/
#include "syscall.h"
#include "../nachos_libc/Printf.h"

int choisirMot(char *motChoisi);
int choixAleatoire(int max);
void afficherMotSecret(int* tabLettres, int nbLettres, char* mot);
int gagne(int* tabLettres, int nbLettres);
int lettreExiste(char l, char* motSecret, int* tabLettres);

int main()
{
    char mot[50];
    int nbLettres = 0;
    int *tabLettres = 0;
    int i;
    int nbCoupsRes = 10;
    char l;

    PutString("Test : Jeu du Pendu !\n\n");

    if ((nbLettres = choisirMot(mot)) == -1)
    {
    	PutString("Erreur dans choisir mot\n");
    	return -1;
    }
    Printf("Mot Choisi : %s de taille %d\n", mot, nbLettres);
    // allocation du tableu qui contiendra 0 si la lettre n'a pas ete trouvee, 1 sinon
    tabLettres = malloc(nbLettres * sizeof(int));

    // erreur allocation
    if (tabLettres == 0)
    {
    	Printf("Erreur allocation du tableau de lettres\n");
        return -1;
    }
    // initialisation du tableau a 0
    for (i = 0 ; i < nbLettres ; i++)
    	tabLettres[i] = 0;

    // tant qu'il reste encore des coups a jouer
    while (nbCoupsRes > 0 && !gagne(tabLettres, nbLettres))
    {
        Printf("\n\nIl reste %d coups a jouer", nbCoupsRes);
        afficherMotSecret(tabLettres, nbLettres, mot);
        Printf("\nEntrer une lettre (en minuscule) : \n");

        Scanf("%c", &l);
        // si pas la bonne lettre, on diminue le nombre de coup restant a jouer
        if (!lettreExiste(l, mot, tabLettres))
        {
        	nbCoupsRes--;
        }
    }

    if (gagne(tabLettres, nbLettres))
        Printf("\nGagne ! Vous avez trouve le mot : %s \n", mot);
    else
        Printf("\nPerdu ! Le mot etait : %s \n", mot);

    free(tabLettres);

    return 0;
}

void afficherMotSecret(int* tabLettres, int nbLettres, char* mot)
{
	int i;
	Printf("\nMot secret :  ");
	for (i = 0 ; i < nbLettres ; i++)
	{
		// lettre trouvee
		if (tabLettres[i])
			PutChar(mot[i]);
		else
			PutChar('*');
	}
	Printf("\n");
}

int gagne(int* tabLettres, int nbLettres)
{
    int i = 0;
    int trouve = 0;

    while (i < nbLettres && (tabLettres[i] != 0))
    {
    	i++;
    }
    if (i == nbLettres)
    	trouve = 1;

    return trouve;
}

int lettreExiste(char l, char* motSecret, int* tabLettres)
{
	int i;
	int trouve = 0;
    for (i = 0 ; motSecret[i] != '\0' ; i++)
    {
    	// lettre existe dans le mot
        if (motSecret[i] == l)
        {
            trouve = 1;
            tabLettres[i] = 1;
        }
    }

    return trouve;
}

/**
 * Choisi un mot aleatoirement dans le fichier Dico.txt
 * renvoie la taille du mot, -1 en cas d'erreur
 */
int choisirMot(char *motChoisi)
{
	// fichier contenant tous les mots du dictionnaire
	OpenFileId idDico = -1;
    int nbMots = 0;
    int indexMot;
    int n = 0;
    char c;
    int i = 0;
   /* int numMotChoisi = 0;
    int i = 0;
    int caractereLu = 0;*/

    // ouverture du fichier contenant les mots
    idDico = Open("dico.txt");

    if (idDico == -1)
    {
        PutString("\nErreur d'ouverture du fichier dico.txt\n");
        return -1;
    }

    // recuperation du nombre de mots inscrit en debut du fichier dico.txt
    do
    {
    	n = Read(&c, sizeof(char), idDico);
    	if(c < '9' && c > '0' && n == 1)
    	{
			nbMots *= 10;
			nbMots += (c - '0');
    	}
    	PutChar(c);
    }while (c < '9' && c > '0' && n == 1);

    // Choix aleatoire de l'indice du mot
    indexMot = choixAleatoire(nbMots);
    // on revient au debut du fichier
    Seek(0, idDico);

    // recherche du mot d'indice indexMot
    if ((n = Read(&c, sizeof(char), idDico)))
	{
    	while ( i < indexMot )
		{
			n = Read(&c, sizeof(char), idDico);
			if(c  == '\n')
			{
				i++;
			}
		}
	}
    else
    {
    	Printf("Erreur read dans recherche index\n");
    	return -1;
    }

    i = 0;
    // copie du mot dans le tableau
    while ((n = Read(&c, sizeof(char), idDico)) && c != '\n')
    {
    	motChoisi[i] = c;
    	i++;
    }
    motChoisi[i] = '\0';

    // fermeture du fichier dico.txt
    Close(idDico);
	return i;
}

int choixAleatoire(int max)
{
    return Random(max)+1;
}

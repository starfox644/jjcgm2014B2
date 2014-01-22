#ifndef __MEM_H
#define __MEM_H
#include "syscall.h"
#define NULL 0
#define MEMORY_SIZE 4096
typedef unsigned int size_t;

struct fb
{
        size_t size;
        struct fb* next;
};
struct fb* head;                // variable globale contenant l'adresse de la premiere structure fb (representant le premier bloc libre)
size_t taille_memoire;          // taille totale de la memoire 
size_t alignement;              // alignement choisi
char* d_memoire;

sem_t sem;

void relacher_verrou();

void prendre_verrou();

/* Choix de la strategie et strategies usuelles */

typedef struct fb* (mem_fit_function_t)(size_t);
mem_fit_function_t* f_mem_fit;

void mem_fit(mem_fit_function_t*);

/*
 * Cette fonction renvoie l'adresse du premier bloc libre de taille suffisante 
 * parametres : tete de la liste chainee des blocs libres et taille de la zone a allouer
 * renvoie l'adresse du bloc libre choisi ou NULL si aucun bloc libre n'est de taille suffisante
*/
struct fb* mem_fit_first(size_t);

/*
 * Cette fonction renvoie l'adresse du bloc libre generant le plus petit residu
 * parametres : tete de la liste chainee des blocs libres et taille de la zone a allouer
 * renvoie l'adresse du bloc libre choisi ou NULL si aucun bloc libre n'est de taille suffisante
*/
struct fb* mem_fit_best(size_t);

/*
 * Cette fonction renvoie l'adresse du bloc libre generant le plus grand residu
 * parametres : tete de la liste chainee des blocs libres et taille de la zone a allouer
 * renvoie l'adresse du bloc libre choisi ou NULL si aucun bloc libre n'est de taille suffisante
 */
struct fb* mem_fit_worst(size_t);


/* initialisation de la liste chainee
*  avec un seul bloc libre de taille passee en parametre
*/
void mem_init(size_t taille);

/*
 * allocation d'une zone memoire 
 * appel a la fonction mem_fit_first
 * prend en parametre la taille de la zone a allouer
 * renvoie l'adresse de la zone allouee (ou l'utilisateur peut ecrire) 
 * ou NULL si allocation impossible (size = 0 ou manque de place)
 */
void* mem_alloc(size_t);


/*
 * Cette fonction gere la liberation d'une zone (verification que l'adresse du bloc a liberer est comprise dans la memoire
 * et que ce bloc est non libre).
 * parametre : adresse de la zone memoire a liberer
 */
void mem_free(void*);

/* 
 * Fonction qui renvoie 1 si la zone d'adresse mem est libre, 0 sinon
 * parametre : adresse de la zone memoire
 */
int est_libre(void* mem);


size_t mem_get_size(void *);

/*
 * Affichage de l'etat de la memoire
 */
void mem_show(void (*print)(void *, size_t, int free));

/*
 * renvoie la taille alignee par rapport a un void* 
 * parametre : taille a alignee
 * renvoie la valeur taille alignee
 */
size_t aligner(size_t t);

/* met a jour le chainage apres l'allocation d'un bloc
 * adresse_ancien_bloc est l'adresse du bloc choisi
 * adresse_new_bloc est l'adresse du nouveau bloc suivant (bloc cree ou suivant de l'ancien bloc)
*/
void maj_chainage_alloc(void* adresse_ancien_bloc, void* adresse_new_bloc);


/*
 * met a jour le chainage en ajoutant le bloc libere
 * fusionne le bloc libere et le bloc suivant et/ou precedent si ceci est possible
 * adresse_bloc est l'adresse du bloc libere (adresse ou commence les donnees)
 */
void maj_chainage_free(char* adresse_bloc);


/* fonctions utilitaires */

/*
 * met a jour le chainage pour ajouter le bloc libere dans la liste et
 * verifie si une fusion avec le bloc libre suivant est possible et effectue cette fusion le cas echeant
 * suiv : adresse de l'en-tete du bloc suivant
 * new_fb : adresse de l'en-tete du bloc libere a ajouter dans la liste
 * ptr_size : adresse de la taille du bloc libere
 */
void test_fusion_droite(struct fb* suiv,  struct fb* new_fb, size_t* ptr_size);

/*
 * met a jour le chainage pour ajouter le bloc libere dans la liste et
 * verifie si une fusion avec le bloc libre precedent est possible et effectue cette fusion le cas echeant
 * prec : adresse de l'en-tete du bloc precedent
 * new_fb : adresse de l'en-tete du bloc libere a ajouter dans la liste
 */
void test_fusion_gauche(struct fb* prec,  struct fb* new_fb);


#endif

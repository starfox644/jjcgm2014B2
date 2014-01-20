#include "mem.h"

void relacher_verrou()
{
    int error;
    // section critique
    error = SemPost(&sem);
    if(error != 0)
    {
        PutString("erreur semaphore post\n");
        Exit(1);
    }
}

void prendre_verrou()
{
    int error;
    // section critique
    error = SemWait(&sem);
    if(error != 0)
    {
        PutString("erreur semaphore wait\n");
        Exit(1);
    }
}

/* initialisation de la liste chainee
*  avec un seul bloc libre de taille passee en parametre
*/
void mem_init(size_t taille)
{
	if(SemInit(&sem, 1) == -1)
	{
		PutString("erreur sem init\n");
		Exit(0);
	}
	taille_memoire = taille;
	char* mem = (char*)Mmap(taille);
	if(mem == NULL)
	{
		PutString("erreur de recuperation de memoire\n");
		Exit(1);
	}
	// stockage du pointeur du tableau memoire afin de placer la premiere structure
	head = (struct fb*) mem;
	// sauvegarde du debut du tableau et de la taille totale de la memoire dans des variables globales
	d_memoire = mem;
	head -> size = taille;
	head -> next = NULL;
	// initialisation de l'alignement
	alignement = sizeof(void*);
    mem_fit(mem_fit_first);
}


/*
 * allocation d'une zone memoire 
 * appel a la fonction mem_fit_first
 * prend en parametre la taille de la zone a allouer
 * renvoie l'adresse de la zone allouee (ou l'utilisateur peut ecrire) 
 * ou NULL si allocation impossible (size = 0 ou manque de place)
 */
void* mem_alloc(size_t size)
{
    prendre_verrou();
    struct fb bloc_choisi;
    size_t* ptr_taille;
    struct fb* ptr_bloc_choisi;
    size_t taille_requise = size + sizeof(size_t);
    size_t taille_restante;
    struct fb* ptr_new_bloc;
    void* retour = NULL;
    
    // allocation d'au moins 1 octet
    if (size > 0)
    {   
        // appel a la methode mem_fit_first afin de recuperer l'adresse du bloc choisi
        ptr_bloc_choisi = f_mem_fit(size);
        
        // allocation impossible, pas de bloc libre de taille suffisante : renvoie NULL
        if(ptr_bloc_choisi == NULL)
        {
            relacher_verrou();
            return NULL;
        }
        // on copie les donnees de la structure du bloc libre dans la structure bloc_choisi 
        // avant ecriture de la taille a la place
        bloc_choisi = *ptr_bloc_choisi;
        
        // ajout du padding si besoin
        taille_requise = aligner(taille_requise);
        
        // on stocke la taille totale du bloc au debut 
        ptr_taille = (size_t*) ptr_bloc_choisi; 
        *ptr_taille = taille_requise; 
        
        // on calcule la taille restante du bloc choisi
        taille_restante =  bloc_choisi.size - taille_requise;
        
        // si on a encore de la place pour au moins la structure fb : on peut creer un nouveau bloc libre
        if ( taille_restante >= sizeof(struct fb) )
        {
                // ecriture de la structure du nouveau bloc 
                ptr_new_bloc = (struct fb*)((char*)ptr_bloc_choisi + taille_requise);
                (*ptr_new_bloc).size = taille_restante;
                (*ptr_new_bloc).next = bloc_choisi.next;
        }
        else
        {
                // pas de nouveau bloc libre, le suivant sera le suivant de l'ancien bloc
                ptr_new_bloc = bloc_choisi.next;
                // ajout de la taille en plus dans le bloc alloue (padding)
                ptr_bloc_choisi->size += taille_restante; 
        }
        // appel a la fonction maj_chainage 
        maj_chainage_alloc(ptr_bloc_choisi, ptr_new_bloc);
        
        // renvoie l'adresse decalee de sizeof(size_t) representant la taille de la zone allouee
        retour = (void*)((char*)ptr_bloc_choisi + sizeof(size_t));
       
        relacher_verrou();
    }
    else if(size == 0)
    {
        relacher_verrou();
        // si demande d'allocation de 0 octet, on alloue 1 octet
        retour = mem_alloc(1);
    }
    else
    {
        // allocation impossible : la taille doit etre > 0
        retour = NULL;
        relacher_verrou();
    }
    return retour;
}


/*
 * Cette fonction renvoie l'adresse du premier bloc libre de taille suffisante 
 * parametres : tete de la liste chainee des blocs libres et taille de la zone a allouer
 * renvoie l'adresse du bloc libre choisi ou NULL si aucun bloc libre n'est de taille suffisante
*/
struct fb* mem_fit_first(size_t size)
{
	// taille necessaire = taille demandee + taille d'un type size_t pour conserver la taille au debut du bloc
	size_t taille_requise = size + sizeof(size_t);
	// initialisation du bloc courant pour le parcours
	struct fb* actu = head;
	
	// recherche du premier bloc libre de taille suffisante
	while (actu != NULL && taille_requise > actu->size)
	{
		// on recupere le bloc suivant 
		actu = actu->next;
	}
	
	// si on a pas trouve de bloc libre de taille suffisante : on renvoie NULL 
	// sinon on renvoie l'adresse du debut du bloc libre choisi
	if (actu == NULL)
		return NULL;
	else
		return actu;
}

/*
 * Cette fonction renvoie l'adresse du bloc libre generant le plus petit residu
 * parametres : tete de la liste chainee des blocs libres et taille de la zone a allouer
 * renvoie l'adresse du bloc libre choisi ou NULL si aucun bloc libre n'est de taille suffisante
*/
struct fb* mem_fit_best(size_t size)
{
    // taille necessaire = taille demandee + taille d'un type size_t pour conserver la taille au debut du bloc
    size_t taille_requise = size + sizeof(size_t);
    // initialisation du bloc courant pour le parcours
    struct fb* actu = head;
    int taille_rest;
    int taille_rest_min = taille_memoire + 1;
    struct fb* fb_min = NULL;
    // recherche du premier bloc libre de taille suffisante
    while (actu != NULL && taille_requise != actu->size)
    {
        // le bloc actuel a la taille suffisante
        if(taille_requise <= actu->size)
        {
            // calcul du residu
            taille_rest = actu->size - taille_requise;
            // si on trouve un residu minimum, on conserve le bloc
            if(taille_rest < taille_rest_min)
            {
                taille_rest_min = taille_rest;
                fb_min = actu;
            }
        }
        // on recupere le bloc suivant 
        actu = actu->next;
    }
    
    // si on a trouve un bloc faisant exactement la bonne taille on le renvoie, sinon on renvoie le minimum
    if(actu != NULL)
    {
        return actu;
    }
    // si on a pas trouve de bloc libre de taille suffisante : on renvoie NULL (fb_min vaut NULL dans ce cas)
    // sinon on renvoie l'adresse du debut du bloc libre choisi
    return fb_min;
}

struct fb* mem_fit_worst(size_t size)
{
    // taille necessaire = taille demandee + taille d'un type size_t pour conserver la taille au debut du bloc
    size_t taille_requise = size + sizeof(size_t);
    // initialisation du bloc courant pour le parcours
    struct fb* actu = head;
    int taille_rest;
    int taille_rest_max = -1;
    struct fb* fb_max = NULL;
    // recherche du premier bloc libre de taille suffisante
    while (actu != NULL)
    {
        // le bloc actuel a la taille suffisante
        if(taille_requise <= actu->size)
        {
            // calcul du residu
            taille_rest = actu->size - taille_requise;
            // si on trouve un residu minimum, on conserve le bloc
            if(taille_rest > taille_rest_max)
            {
                taille_rest_max = taille_rest;
                fb_max = actu;
            }
        }
        // on recupere le bloc suivant 
        actu = actu->next;
    }
    
    // si on a pas trouve de bloc libre de taille suffisante : on renvoie NULL (fb_max vaut NULL dans ce cas)
    // sinon on renvoie l'adresse du debut du bloc libre choisi
    return fb_max;
}

/*
 * Cette fonction gere la liberation d'une zone (verification que l'adresse du bloc a liberer est comprise dans la memoire
 * et que ce bloc est non libre).
 * parametre : adresse de la zone memoire a liberer
 */
void mem_free(void* mem)
{
    prendre_verrou();
    // si l'adresse est comprise dans la memoire et que le bloc est non libre : mise a jour du chainage et liberation 
    if((char*)mem >= d_memoire && (char*)mem < d_memoire + taille_memoire) 
    {
        if(!est_libre(mem))
            maj_chainage_free((char*) mem);
    }
    relacher_verrou();
}

/* 
 * Fonction qui renvoie 1 si la zone d'adresse mem est libre, 0 sinon
 * parametre : adresse de la zone memoire
 */
int est_libre(void* mem)
{
    struct fb* actu;
    actu = head;
    while (actu != NULL && (char*)actu != (char*) mem)
        actu = actu->next;
    
    return (actu != NULL);
}

       
size_t mem_get_size(void * mem)
{
    prendre_verrou();
    size_t retour;
    size_t* ptr_size = (size_t*)((char*)mem - sizeof(size_t));
    retour = (*ptr_size) - sizeof(size_t);
    relacher_verrou();
    return retour;
}


/*
 * Affichage de l'etat de la memoire
 */
void mem_show(void (*print)(void *, size_t, int free))
{
    prendre_verrou();
    int libre;
    size_t* ptr_taille;
    struct fb* actu;
    char* ptr_bloc = NULL;
    
    // si la memoire commence par un bloc libre : on l'affiche
    if (d_memoire == (char*)head)
    {
    	libre = 1;
        // affichage du premier bloc libre
        print(head, head->size, 1);
        libre = (libre+1) % 2;
        actu = head->next;
        // initialisation de pointeur de bloc
        ptr_bloc = d_memoire + head->size;
    }
    else
    {
    	libre = 0;
        ptr_taille = (size_t*) d_memoire;
        // affichage du premier bloc occupe (avant la tete)
        print(d_memoire, *ptr_taille, libre);
        
        // si le bloc suivant est la tete 
        if (((char*)d_memoire + *ptr_taille) == (char*) head)
        {
        	libre = (libre+1) % 2;
        }
        else
        {
                // on initialise ptr_bloc au bloc occupe suivant
                ptr_bloc = (char*)d_memoire + *ptr_taille;
        }
        actu = head;
                    
    }
    
    // parcours de la memoire jusqu'a la fin
    while (ptr_bloc < (d_memoire + taille_memoire))
    {
        // si le bloc actu est un bloc libre
        if (libre)
        {
            // affichage du bloc libre actuel et passage au suivant
            print(actu, actu->size, 1);
            // deplacement de ptr_bloc vers le bloc suivant
            ptr_bloc = (char*)actu + actu->size; 
            // on passe au bloc libre suivant
            actu = actu->next;
        }
        else
        {
            // affichage des blocs occupes jusqu'au prochain bloc libre ou jusqu'a la fin de la memoire
            do
            {
                // Pour le premier cas on verifie qu'on est pas a la fin de la memoire
                if(ptr_bloc < (d_memoire + taille_memoire))
                {
                        // recuperation de l'adresse de la taille du bloc occupe
                        ptr_taille = (size_t*)ptr_bloc; 
                        // affichage du bloc occupe
                        print(ptr_bloc, *ptr_taille, libre);
                }
                // on passe au bloc suivant
                ptr_bloc = (char*) ptr_bloc + *ptr_taille;
            }while ((char*)ptr_bloc != (char*)actu && ptr_bloc < (d_memoire + taille_memoire));
        }
        libre = (libre+1) % 2;
    }// fin while
    relacher_verrou();
}


/*
 * renvoie la taille alignee par rapport a un void* 
 * parametre : taille a alignee
 * renvoie la valeur taille alignee
 */
size_t aligner(size_t size)
{
	size_t taille_adr = sizeof(void*);
	return (size + (taille_adr - 1)) & (~(taille_adr - 1));
}


/* met a jour le chainage apres l'allocation d'un bloc
 * adresse_ancien_bloc est l'adresse du bloc choisi
 * adresse_new_bloc est l'adresse du nouveau bloc suivant (bloc cree ou suivant de l'ancien bloc)
*/
void maj_chainage_alloc(void* adresse_bloc_choisi, void* adresse_new_bloc)
{
	// initialisation du bloc courant pour le parcours
	struct fb* actu = head;
	// si le bloc choisi est la tete de la liste de blocs libres : le nouveau bloc devient la tete
	if (adresse_bloc_choisi == head)
	{
		head = adresse_new_bloc;
	}
	else
	{
		// recherche du bloc precedent du bloc choisi
		while (actu != NULL && actu->next != adresse_bloc_choisi)
		{
			// on recupere le bloc suivant 
			actu = actu->next;
		}
		// mise a jour du suivant du bloc trouve
		actu->next = adresse_new_bloc;
	}
}

/*
 * met a jour le chainage en ajoutant le bloc libere
 * fusionne le bloc libere et le bloc suivant et/ou precedent si ceci est possible
 * adresse_bloc est l'adresse du bloc libere (adresse ou commence les donnees)
 */
void maj_chainage_free(char* adresse_bloc)
{
    struct fb* actu;
    struct fb* prec;
    struct fb* new_fb;
    
    // recuperation adresse de la structure du bloc libere
    new_fb = (struct fb*)(adresse_bloc - sizeof(size_t));
    // recuperation de la taille totale du bloc libere
    size_t* ptr_size = (size_t*)(adresse_bloc - sizeof(size_t));
    
    // si la tete est a NULL, il n'y a plus de bloc libre
    if(head == NULL)
    {    
        // calcul de la taille du nouveau bloc 
        new_fb->size = *ptr_size;
        // mise a jour du suivant du nouveau bloc
        new_fb->next = NULL;
        // le bloc libere devient la tete
        head = new_fb;
    }
    // s'il y a au moins un bloc libre
    else
    {
        
        // on regarde si le bloc suivant est libre ou pas
        actu = head;
        prec = NULL;
        // parcours des zones libres pour recuperer le bloc precedant le bloc libere
        while ( actu->next != NULL && ( (char*) actu < adresse_bloc ) )
        {
            prec = actu;
            actu = actu->next;
        }
        
        // cas particuliers ou la boucle s'arrete tout de suite
        if(prec == NULL)
        {
            // le bloc libere est avant les autres blocs libres, il devient la tete
            if((char*)actu > adresse_bloc)
            {
                test_fusion_droite(actu, new_fb, ptr_size);
                head = new_fb;
            }
            // il n'y a que la tete de libre : on ajoute a la suite
            else
            {
                // fusion du bloc libere et de la tete
                if((char*)head + head->size == (char*)new_fb)
                {
                    // calcul de la taille du nouveau bloc 
                    head->size += *ptr_size;
                }
                else
                {
                    // le bloc libre suivant n'est pas adjacent : pas de fusion mais mise a jour du chainage
                    new_fb->size = *ptr_size;
                    // mise a jour du suivant du nouveau bloc
                    new_fb->next = NULL;
                    // le bloc libere devient le suivant de la tete
                    head->next = new_fb;
                }
            }
                    
        }
        // ajout au milieu : si le bloc a au moins un bloc libre avant et un apres
        else if (actu != NULL && (char*)actu > (char*)new_fb)//&& prec != head)
        {
           
           test_fusion_droite(actu, new_fb, ptr_size);
           test_fusion_gauche(prec, new_fb);
        }
        // ajout en queue : le bloc libere est situe apres tous les blocs libres
        else
        {
           // taille du bloc libere
           new_fb->size = *ptr_size;
           // le bloc libere devient le dernier bloc
           new_fb->next = NULL;
           test_fusion_gauche(actu, new_fb);
        }
    }
}

/*
 * met a jour le chainage pour ajouter le bloc libere dans la liste et
 * verifie si une fusion avec le bloc libre suivant est possible et effectue cette fusion le cas echeant
 * suiv : adresse de l'en-tete du bloc suivant
 * new_fb : adresse de l'en-tete du bloc libere a ajouter dans la liste
 * ptr_size : adresse de la taille du bloc libere
 */
void test_fusion_droite(struct fb* suiv,  struct fb* new_fb, size_t* ptr_size)
{
    // recuperation de l'adresse du bloc suivant
    char* ptr_bloc_suiv = (char*) new_fb + *ptr_size;
    
    // si la tete est adjacente : fusion avec le bloc libre de droite
    if ( (char*)suiv == ptr_bloc_suiv )
    {
        // calcul de la taille du nouveau bloc 
        new_fb->size = *ptr_size + suiv->size;
        // mise a jour du suivant du nouveau bloc
        new_fb->next = suiv->next;
    }
    else
    {
        // le bloc libre suivant n'est pas adjacent : pas de fusion mais mise a jour du chainage
        new_fb->size = *ptr_size;
        // mise a jour du suivant du nouveau bloc
        new_fb->next = suiv;
    }	
}

/*
 * met a jour le chainage pour ajouter le bloc libere dans la liste et
 * verifie si une fusion avec le bloc libre precedent est possible et effectue cette fusion le cas echeant
 * prec : adresse de l'en-tete du bloc precedent
 * new_fb : adresse de l'en-tete du bloc libere a ajouter dans la liste
 */
void test_fusion_gauche(struct fb* prec,  struct fb* new_fb)
{
    // si le bloc libre precedent est adjacent : fusion 
    if ( ((char*)prec + prec->size) == (char*)new_fb )
    {
        // calcul de la nouvelle taille du bloc precedent 
        prec->size += new_fb->size; 
        // mise a jour du chainage 
        prec->next = new_fb->next;
    }
    else
    {
        // mise a jour du chainage : le suivant du bloc libre precedent est le bloc libere
        prec->next = new_fb;
    }
}


void mem_fit(mem_fit_function_t* f)
{
    prendre_verrou();
    f_mem_fit = f;
    relacher_verrou();
}

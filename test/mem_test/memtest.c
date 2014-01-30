#include "../malloc/mem.h"

#define TAILLE_BUFFER 128
#define TAILLE_MEMOIRE 4096

#define MAX_ALLOC 1024
static int nb_free, nb_busy;
static void *allocs[MAX_ALLOC];
static unsigned int sizes[MAX_ALLOC];
static unsigned int free_size[MAX_ALLOC];
static unsigned int busy_size[MAX_ALLOC];
static int position;
static unsigned int mem_align=0;

void decompte_zone(void *adresse, size_t taille, int free) {
    if (free)
        free_size[nb_free++] = taille;
    else
        busy_size[nb_busy++] = taille;
}

void afficher_zone(void *adresse, size_t taille, int free) {
    decompte_zone(adresse, taille, free);
    PutString("\n");
    PutString("zone ");
    if(free)
    	PutString(" libre ");
    else
    	PutString(" occupee ");
    PutString(" adresse : ");
    PutInt((unsigned int)adresse);
    PutString(" taille : ");
    PutInt(taille);
    PutString("\n");
    /*printf("Zone %s, Adresse : %lx, Taille : %lu\n", free?"libre":"occupee",
           (unsigned long) adresse, (unsigned long) taille);*/
}

void my_mem_init(size_t s) {
    position = 0;
    mem_init(s);
}

void *my_mem_alloc(size_t s) {
    void *result = mem_alloc(s);
    sizes[position] = s;
    allocs[position++] = result;
    PutString("Allocation de ");
    PutInt((unsigned long) s);
    PutString("octets en ");
    PutInt((unsigned long) result);
    PutChar('\n');
    mem_align |= (unsigned int)result;
    return result;
}

void my_mem_free(int num) {
    //printf("Liberation de la zone en %lx\n", (unsigned long) allocs[num]);
    mem_free(allocs[num]);
}

void my_mem_show(void (*print)(void *, size_t, int free)) {
    nb_free = 0;
    nb_busy = 0;
    mem_show(print);
}

int main()
{
  size_t struct_fb_size, struct_bb_size;
  int i;

  my_mem_init(TAILLE_MEMOIRE);
  //printf("Test de l'etat initial de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 1)
  {
	  PutString("assert failed ! 78\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

  //printf("Test de base, serie d'allocations :\n");
  my_mem_alloc(8);
  my_mem_alloc(16);
  my_mem_alloc(4);
  my_mem_alloc(12);
  PutString("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 1 || nb_busy != 4)
  {
	  PutString("assert failed ! 92\n");
	  Exit(0);
  }
 // printf("TEST OK\n\n");

 // printf("Test de mem_init :\n");
  my_mem_init(TAILLE_MEMOIRE);
//  printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 1 || nb_busy != 0)
  {
	  PutString("assert failed ! l89\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

//  printf("Test d'allocation puis liberation :\n");
  my_mem_alloc(4);
  my_mem_alloc(4);
  my_mem_alloc(30);
  my_mem_alloc(1);
  my_mem_alloc(64);
  my_mem_alloc(24);
  my_mem_alloc(23);
  my_mem_free(2);
 // printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 2 || nb_busy != 6)
  {
	  PutString("assert failed ! l108\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

 // printf("Verification de la taille des allocations (multiples de %zd)\n",
//	 sizeof(void*));
  /*for (i=0; i<nb_busy; i++)
      assert((busy_size[i] & (sizeof(void*)-1)) == 0);*/
 // printf("TEST OK\n\n");
  {
    int al=1;
    unsigned int tmp = ~mem_align;
    while (tmp & 1) {
      tmp >>= 1;
      al <<= 1;
    }
//    printf("L'alignement semble être systématique sur %i octets\n", i);
  }

 // printf("Test de fusion amont :\n");
  my_mem_free(1);
 // printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  struct_fb_size = free_size[0] - 36;
  if(nb_free != 2 || nb_busy != 5)
  {
	  PutString("assert failed ! 134\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

 // printf("Test de fusion aval :\n");
  my_mem_free(3);
//  printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 2 || nb_busy != 4)
  {
	  PutString("assert failed ! 144\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

 // printf("Test de fusion amont+aval :\n");
  my_mem_free(5);
  my_mem_show(decompte_zone);
  if(nb_free != 3 || nb_busy != 3)
  {
	  PutString("assert failed ! 153\n");
	  Exit(0);
  }
  my_mem_free(4);
  //printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 2 || nb_busy != 2)
  {
	  PutString("assert failed ! 160\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

 // printf("Récupération de la taille d'entête de bloc occupé:\n");
  my_mem_init(TAILLE_MEMOIRE);
  my_mem_show(decompte_zone);
  struct_bb_size = free_size[0];
  my_mem_alloc(__BIGGEST_ALIGNMENT__);
  my_mem_show(decompte_zone);
  struct_bb_size -= free_size[0] + __BIGGEST_ALIGNMENT__;
 /* printf("Apparemment la taille de votre entete de bloc occupe est de %lu "
         "octets\n", (unsigned long) struct_bb_size);*/
 // printf("\n");
  
  //printf("Test d'allocation/liberation de tout l'espace :\n");
  my_mem_init(TAILLE_MEMOIRE);
  my_mem_alloc(TAILLE_MEMOIRE - struct_bb_size);
 // printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 0 || nb_busy != 1)
  {
	  PutString("assert failed ! 182\n");
	  Exit(0);
  }
  my_mem_free(0);
 // printf("Etat de la memoire :\n");
  my_mem_show(afficher_zone);
  if(nb_free != 1 || nb_busy != 0)
  {
	  PutString("assert failed ! 189\n");
	  Exit(0);
  }
  PutString("TEST OK\n\n");

//  printf("Récupération de la taille d'entête de bloc libre:\n");
  my_mem_init(TAILLE_MEMOIRE);
  my_mem_alloc(4*__BIGGEST_ALIGNMENT__);
  my_mem_alloc(TAILLE_MEMOIRE - 2*struct_bb_size - 4*__BIGGEST_ALIGNMENT__);
  my_mem_show(afficher_zone);
  if(nb_free != 0 || nb_busy != 2)
  {
	  PutString("assert failed ! 210\n");
	  Exit(0);
  }
  my_mem_free(0);
  my_mem_show(decompte_zone);
  if(nb_free != 1 || nb_busy != 1)
  {
	  PutString("assert failed ! 217\n");
	  Exit(0);
  }
  for (i=1; i<4*__BIGGEST_ALIGNMENT__; i++) {
    my_mem_alloc(4*__BIGGEST_ALIGNMENT__ - i);
    my_mem_show(decompte_zone);
    if((nb_free != 1 && nb_free != 0) || nb_busy != 2)
    {
    	PutString("assert failed ! 221\n");
    }
    if (nb_free == 1) {
	break;
    }
    my_mem_free(i+1);
  }
  
  return 0;
}

#ifdef CHANGED
#ifdef step4
#include "addrSpaceAllocator.h"
#include "copyright.h"
#include "system.h"


AddrSpaceAllocator::AddrSpaceAllocator(AddrSpace* addrS, int addr, int length)
{
	ASSERT(addr % PageSize == 0);
	ASSERT(length % PageSize == 0);

	this->addrspace = addrS;
	freeHead = new struct space;
	freeHead->addr = addr;
	freeHead->length = length;
	freeHead->next = NULL;
	busyHead = new struct space;
	busyHead = NULL;
	s_alloc = new Semaphore("semaphore alloc", 1);
}

AddrSpaceAllocator::~AddrSpaceAllocator()
{
	// liberation de la liste des blocs libres
	struct space* actu = freeHead;
	struct space* tmp = NULL;
	while (actu != NULL)
	{
		tmp = actu;
		actu = actu->next;
		delete tmp;
	}
	// liberation de la liste des blocs occupes
	actu = busyHead;
	tmp = NULL;
	while (actu != NULL)
	{
		tmp = actu;
		actu = actu->next;
		delete tmp;
	}
	delete s_alloc;
}

/*
	 *	Affichage de la liste des blocs libres
	 */
void AddrSpaceAllocator::printFreeList()
{
	struct space* actu = freeHead;
	if (actu == NULL)
	{
		Printf("La liste des bloc libre est vide\n");
	}
	else
	{
		Printf("Contenu de la liste des blocs libres : \n ");
		while (actu != NULL)
		{
			Printf("(addr = %d, length = %d)  " , actu->addr, actu->length);
			actu = actu->next;
		}
	}
	Printf("\n\n");
}

/*
 * Affichage de la liste des blocs occupes
 */
void AddrSpaceAllocator::printBusyList()
{
	struct space* actu = busyHead;
	if (actu == NULL)
	{
		Printf("La liste des bloc occupes est vide\n");
	}
	else
	{
		Printf("Contenu de la liste des blocs occupes : \n ");
		while (actu != NULL)
		{
			Printf("(addr = %d, length = %d, fp = %d)  \n" , actu->addr, actu->length, actu->forbiddenPage);
			actu = actu->next;
		}
	}
	Printf("\n\n");
}

/*
 * Verification qu'il est possible d'allouer un bloc dont la taille
 * est passe en parametre
 * Si une allocation est possible un pointeur vers le bloc est retourne,
 * et l'allocation pourra etre realisee,
 * NULL sinon.
 */
struct space* AddrSpaceAllocator::canAllocate(int length)
{
	struct space* current = freeHead;
	// recherche du premier bloc libre de taille suffisante
	while (current != NULL && current->length < length)
	{
		current = current->next;
	}
	// plus de bloc libre ou memoire insuffisante
	if (current == NULL)
	{
		return NULL;
	}
	else
	{
		return current;
	}
}

/*
 *	Renvoie vrai si l'adresse est dans une pile, faux sinon
 */
bool AddrSpaceAllocator::isInStack(int addr)
{
	struct space* actu = busyHead;

	//parcours pour recherche le bloc contenant l'adresse addr
	while (actu != NULL && !adresseDansBloc(addr, actu))
	{
		actu = actu->next;
	}
	// addr n'est present dans aucun bloc
	if (actu == NULL || !actu->forbiddenPage)
		return false;
	else
		return true;
}

/*
 *	Renvoie vrai si l'adresse addr passee en parametre appartient au bloc actu,
 *	faux sinon.
 */
bool AddrSpaceAllocator::adresseDansBloc(int addr, struct space* actu)
{
	return (actu->addr <= addr && addr <= (actu->addr + actu->length));
}

/*
 * suppression du bloc libre a l'adresse addr et de taille lengthAlloc
 */
void AddrSpaceAllocator::removeFreeSpace(int addr, int lengthAlloc)
{
	int tailleRestante;
	struct space* actu = freeHead;
	struct space* prec = NULL;

	while(actu != NULL && actu->addr != addr )
	{
		prec = actu;
		actu = actu->next;
	}
	// L'element d'adresse addr doit etre dans la liste (verification realisee avant)
	ASSERT(actu != NULL);
	tailleRestante = actu->length - lengthAlloc;
	// calcul taille restante entre la taille du bloc libre et la taille a allouer
	if (tailleRestante > 0)
	{
		// modif bloc avec nouvelle adresse et taille restante
		actu->length = tailleRestante;
		actu->addr = addr + lengthAlloc;
	}
	else
	{
		// suppression de la tete
		if (prec == NULL)
		{
			freeHead = actu->next;
		}
		else
		{
			// suppresion du bloc
			prec->next = actu->next;
		}
		//delete actu;
	}
}

/*
 * Ajout d'un bloc libre de taille length a l'adresse addr
 */
void AddrSpaceAllocator::addFreeSpace(int addr, int length)
{
	struct space* actu = freeHead;
	struct space* prec = NULL;
	struct space* newSpace = new struct space;

	newSpace->addr = addr;
	newSpace->length = length;
	newSpace->next = NULL;

	// la liste des blocs libres est vide
	if(freeHead == NULL)
	{
		freeHead = newSpace;
	}
	else
	{
        // parcours des zones libres pour recuperer le bloc precedant le bloc libere
        while ( actu->next != NULL && (actu->addr < addr) )
        {
            prec = actu;
            actu = actu->next;
        }
		if(prec == NULL)
		{
			// le bloc libere est avant les autres blocs libres, il devient la tete
			if(actu->addr > addr)
			{
				// Verification : actu doit etre la tete
				ASSERT(actu == freeHead);
				testFusionDroite(actu, newSpace);
				freeHead = newSpace;
			}
			// il n'y a que la tete de libre : on ajoute a la suite
			else
			{
				// fusion du bloc libere et de la tete
				if((freeHead->addr + freeHead->length) == newSpace->addr)
				{
					// calcul de la taille du nouveau bloc
					freeHead->length += length;
				}
				else
				{
					// Pas de fusion avec la tete :
					//le bloc libere devient le suivant de la tete
					freeHead->next = newSpace;
				}
			}

		}
		// ajout au milieu : si le bloc a au moins un bloc libre avant et un apres
		else if (actu != NULL && actu->addr > newSpace->addr)
		{
		   testFusionDroite(actu, newSpace);
		   testFusionGauche(prec, newSpace);
		}
		// ajout en queue : le bloc libere est situe apres tous les blocs libres
		else
		{
		   testFusionGauche(actu, newSpace);
		}
	}
}

/*
 *	Suppression du bloc occupe d'adresse addr
 *	Renvoie la taille du bloc libere, -1 en cas d'erreur.
 *	addr = debut page interdite s'il y a
 */
struct space* AddrSpaceAllocator::removeBusySpace(int addr)
{
	struct space* actu = busyHead;
	struct space* prec = NULL;
	struct space* res = NULL;

	// la liste des blocs occupes est vide : erreur
	if (actu == NULL)
		return NULL;

	while(actu != NULL && (actu->addr < addr))
	{
		prec = actu;
		actu = actu->next;
	}
	// si la liste est vide ou que l'element n'est pas present dans la liste
	if(actu == NULL || actu->addr != addr)
	{
		Printf("Bloc d'adresse %d pas trouve\n", addr);
		return NULL;
	}
	else if (actu != NULL && actu == busyHead && actu->addr < addr)
	{
		return NULL;
	}
	else
	{
		res = actu;
		// l'element a supprimer est en tete
		if(prec == NULL)
		{
			// si c'est le seul element
			if(actu->next == NULL)
			{
				busyHead = NULL;
			}
			else
			{
				busyHead = actu->next;
			}
		}
		// l'element est a la fin de la liste
		else if(actu->next == NULL)
		{
			prec->next = NULL;
		}
		else
		{
			prec->next = actu->next;
		}
		//delete actu;
		return res;
	}
}

/*
 *	Ajout d'un bloc occupe a l'adresse addr, de taille lengthAlloc
 *	et contenant ou non une page interdite. (ca presence est indiquee par
 *	le booleen forbiddenPage)
 *	Addr = adresse debut page interdite s'il y a
 */
void AddrSpaceAllocator::addBusySpace(int addr, int lengthAlloc, bool forbiddenPage)
{
	struct space* actu = busyHead;
	struct space* prec = NULL;

	// creation du nouveau bloc occupe
	struct space* newSpace = new struct space;
	newSpace->addr = addr;
	newSpace->length = lengthAlloc;
	newSpace->forbiddenPage = forbiddenPage;
	newSpace->next = NULL;

	while (actu != NULL && actu->addr < addr)
	{
		prec = actu;
		actu = actu->next;
	}

	// si la liste des blocs occupes est vide
	if (prec == NULL && actu == NULL)
	{
		busyHead =newSpace;
	}
	// ajout en queue
	else if (actu == NULL)
	{
		newSpace->next = NULL;
		prec->next = newSpace;
	}
	// ajout en tete
	else if (actu == busyHead)
	{
		newSpace->next = actu;
		busyHead = newSpace;
	}
	else
	{
		newSpace->next = actu;
		prec->next = newSpace;
	}
}

/*
 *	Verifie qu'une fusion est possible entre le bloc newSpace
 *	et le precedent nomme prec.
 *	Mise a jour du chainage en consequence
 */
void AddrSpaceAllocator::testFusionGauche(struct space* prec,  struct space* newSpace)
{
    // si le bloc libre precedent est adjacent : fusion
    if ( (prec->addr + prec->length) == newSpace->addr)
    {
        // calcul de la nouvelle taille du bloc precedent
        prec->length += newSpace->length;
        // mise a jour du chainage
        prec->next = newSpace->next;
    }
    else
    {
        // mise a jour du chainage : le suivant du bloc libre precedent est le bloc libere
        prec->next = newSpace;
    }
}

/*
 *	Verifie qu'une fusion est possible entre le bloc newSpace
 *	et le suivant nomme suiv.
 *	Mise a jour du chainage en consequence
 */
void AddrSpaceAllocator::testFusionDroite(struct space* suiv,  struct space* newSpace)
{
	int length = newSpace->length;
    // Test fusion avec le bloc libre de droite
    if ( suiv->addr == newSpace->addr + length )
    {
        // calcul de la taille du nouveau bloc
        newSpace->length += suiv->length;
        // mise a jour du suivant du nouveau bloc
        newSpace->next = suiv->next;
    }
    else
    {
        // le bloc libre suivant n'est pas adjacent : pas de fusion mais mise a jour du chainage
        newSpace->next = suiv;
    }
}

/*
 *	Allocation d'un bloc memoire, renvoie  l'adresse du bloc si elle s'est
 *	bien passee, -1 sinon. Cette adresse represente le debut de la page interdite
 *	s'il y a.
 *	lengthAlloc : taille du bloc a allouee (+ taille page interdite, s'il y a)
 *					cette taille n'est pas alignee selon les pages.
 *	write : si ce booleen est a vrai, les pages seront allouees en ecriture
 *	forbiddenPage : Indique s'il y a besoin ou non de creer une page interdite
 *					en debut de bloc
 *
 */
int AddrSpaceAllocator::allocateFirst(int lengthAlloc, bool write, bool forbiddenPage)
{
	int alignedLength = divRoundUp(lengthAlloc, PageSize) * PageSize;
	struct space* current = canAllocate(alignedLength);
	int size, addrMap = 0;
	s_alloc->P();
	if (current == NULL)
	{
		s_alloc->V();
		return -1;
	}
	else
	{
		// calcul de la taille a allouer en fonction de la presence ou non
		// d'une page interdite
		if (forbiddenPage)
		{
			size = alignedLength - PageSize;
			addrMap = current->addr + PageSize;
			current->forbiddenPage = true;
		}
		else
		{
			size = alignedLength;
			addrMap = current->addr;
			current->forbiddenPage = false;
		}
		// associe des frame physique a l'espace d'adressage
		DEBUG(',', "MAP STACK\n");
		if (addrspace->mapMem(addrMap, size, write))
		{
			addBusySpace(current->addr,alignedLength, forbiddenPage);
			// suppression du bloc libre et ajout eventuel d'un nouveau bloc
			removeFreeSpace(current->addr, alignedLength);
		}
		else
		{
			s_alloc->V();
			return -1;
		}
	}
	s_alloc->V();
	return addrMap;
}

/*
 *	Liberation du bloc occupe d'adresse addr, renvoie 0 si elle s'est
 *	bien passee, -1 sinon.
 *	addr : represente l'adresse, alignee sur la taille d'une page,
 *	du debut du bloc. (s'il y a une page interdite en debut du bloc, addr
 *	sera celle du debut de cette page)
 */
int AddrSpaceAllocator::free(int addr)
{
	struct space* actu = NULL;
	int addrBloc;
	ASSERT((addr%PageSize) == 0);
	s_alloc->P();
	// suppression dans la liste des bloc occupes (addr = debut page interdite s'il y a)
	if ((actu = removeBusySpace(addr)) == NULL)
	{
		s_alloc->V();
		ASSERT(FALSE);
		return -1;
	}
	else
	{
		ASSERT(actu->length%PageSize == 0);
		DEBUG(',', "UNMAP STACK\n");
		if (actu->forbiddenPage)
		{
			addrBloc = actu->addr+PageSize;
		}
		else
		{
			addrBloc = actu->addr;
		}
		// appel a unMapMem avec l'adresse du debut du bloc et la taille sans la page interdite
		if(addrspace->unMapMem(addrBloc/PageSize, (actu->length - PageSize)/PageSize))
		{
			//ajout dand la liste des blocs libres
			addFreeSpace(addr,actu->length);
			s_alloc->V();
			return 0;
		}
		else
		{
			ASSERT(FALSE);
			s_alloc->V();
			return -1;
		}
	}
}
#endif // step 4
#endif // CHANGED

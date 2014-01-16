#ifdef CHANGED
#ifndef ___ADDRSPACEALLOCATOR
#define ___ADDRSPACEALLOCATOR

class AddrSpace;

	struct space
	{
		int addr;
		int length;
		bool forbiddenPage;
		struct space* next;
	};

class AddrSpaceAllocator
{
	public:
		/*
		 * creation addrSpaceAllocator
		 */
		AddrSpaceAllocator(AddrSpace* addrS, int addr, int length);

		/*
		 * destructeur
		 */
		~AddrSpaceAllocator();

		/*
		 * Affichage de la liste des blocs libres
		 */
		void printFreeList();

		/*
		 *	Affichage de la liste des blocs occupes
		 */
		void printBusyList();

		/*
		 * Verification qu'il est possible d'allouer un bloc dont la taille
		 * est passe en parametre
		 * Si une allocation est possible un pointeur vers le bloc est retourne,
		 * et l'allocation pourra etre realisee,
		 * NULL sinon.
		 */
		struct space* canAllocate(int length);

		/*
		 *	Allocation d'un bloc memoire, renvoie  l'adresse du bloc si elle s'est
		 *	bien passee, -1 sinon.
		 *	lengthAlloc : taille du bloc a allouee (+ taille page interdite, s'il y a)
		 *					cette taille n'est pas alignee selon les pages.
		 *	write : si ce booleen est a vrai, les pages seront allouees en ecriture
		 *	forbiddenPage : Indique s'il y a besoin ou non de creer une page interdite
		 *					en debut de bloc
		 *
		 */
		int allocateFirst(int lengthAlloc, bool write, bool forbiddenPage);

		/*
		 *	Liberation du bloc occupe d'adresse addr, renvoie 0 si elle s'est
		 *	bien passee, -1 sinon.
		 *	addr : represente l'adresse, alignee sur la taille d'une page,
		 *	du debut du bloc. (s'il y a une page interdite en debut du bloc, addr
		 *	sera l'adresse du prochain octet apres cette page)
		 */
		int free(int addr);

	private :

		/*
		 * suppression du bloc libre a l'adresse addr et de taille lengthAlloc
		 */
		void removeFreeSpace(int addr, int lengthAlloc);

		/*
		 * Ajout d'un bloc libre de taille length a l'adresse addr
		 */
		void addFreeSpace(int addr, int length);

		/*
		 *	Suppression du bloc occupe d'adresse addr
		 *	Renvoie la taille du bloc libere, -1 en cas d'erreur.
		 */
		int removeBusySpace(int addr);

		/*
		 *	Ajout d'un bloc occupe a l'adresse addr, de taille lengthAlloc
		 *	et contenant ou non une page interdite. (ca presence est indiquee par
		 *	le booleen forbiddenPage)
		 */
		void addBusySpace(int addr, int lengthAlloc, bool forbiddenPage);

		/*
		 *	Verifie qu'une fusion est possible entre le bloc newSpace
		 *	et le precedent nomme prec.
		 */
		void testFusionGauche(struct space* prec,  struct space* newSpace);

		/*
		 *	Verifie qu'une fusion est possible entre le bloc newSpace
		 *	et le suivant nomme suiv.
		 *	Mise a jour du chainage en consequence
		 */
		void testFusionDroite(struct space* suiv,  struct space* newSpace);


		AddrSpace* addrspace;

		/*
		 * tetes des listes de blocs libres et occupes
		 */
		struct space* freeHead;
		struct space* busyHead;
};
#endif // ___ADDRSPACEALLOCATOR


#endif // CHANGED

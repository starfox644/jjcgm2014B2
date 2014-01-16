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
		 *	Allocation d'un bloc memoire.
		 *	lengthAlloc :
		 */
		int allocateFirst(int lengthAlloc, bool write, bool forbiddenPage);
		int free(int addr);
		struct space* canAllocate(int length);

	private :
		void removeFreeSpace(int addr, int lengthAlloc);
		void addBusySpace(int addr, int lengthAlloc, bool forbiddenPage);
		int removeBusySpace(int addr);
		void addFreeSpace(int addr, int length);
		void testFusionGauche(struct space* prec,  struct space* newSpace);
		void testFusionDroite(struct space* suiv,  struct space* newSpace);

		AddrSpace* addrspace;
		struct space* freeHead;
		struct space* busyHead;
};
#endif // ___ADDRSPACEALLOCATOR


#endif // CHANGED

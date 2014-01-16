#ifdef step4
#include "frameProvider.h"
#include "machine.h"
#include "system.h"


FrameProvider::FrameProvider ()
{
	bitmap = new BitMap(NumPhysPages);
}

int FrameProvider::GetEmptyFrame()
{
	int indexFrame;
	if ((indexFrame = bitmap->Find ()) == -1)
	{
		DEBUG(',', "Plus de frames physiques disponibles\n");
		return -1;
	}
	// initialisation du cadre à 0
	bzero (&(machine->mainMemory[indexFrame*PageSize]), PageSize);
	// indique que la page d’index “indexFrame” est occupee
	bitmap->Mark(indexFrame);
	return indexFrame;
}

bool FrameProvider::ReleaseFrame(int indexFrame)
{
	if(!bitmap->Test(indexFrame))
	{
		DEBUG(',', "Echec de liberation de la frame physique %i non allouee\n", indexFrame);
		ASSERT(FALSE);
		return false;
	}
	else
	{
		DEBUG(',', "liberation de la frame physique %i\n", indexFrame);
		// indique que la page est libre
		bitmap->Clear(indexFrame);
		return true;
	}
}


int FrameProvider::NumAvailFrame()
{
	// retourne le nombre de cadres libres
	return bitmap->NumClear();
}

#endif

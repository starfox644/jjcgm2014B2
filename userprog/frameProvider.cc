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
		printf(" No frame are free\n");
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
		return false;
	}
	else
	{
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

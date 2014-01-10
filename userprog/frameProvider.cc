#ifdef step4
#include "frameProvider.h"


FrameProvider::FrameProvider ()
{
	bitmap = new BitMap(NumPhysPages);
}

int FrameProvider::GetEmptyFrame()
{
	int indexFrame;
	if ((indexFrame = bitmap->Find ()) == -1)
	{
		printf(" No frame are free ");
		return -1;
	}
	// initialisation du cadre à 0
	bzero (machine->mainMemory[indexFrame*PageSize], PageSize);
	// indique que la page d’index “indexFrame” est occupee
	bitmap->Mark(indexFrame);
	return (indexFrame * PageSize);
}

void FrameProvider::ReleaseFrame(int addrFrame)
{
	int index = addrFrame / PageSize;
	// indique que la page est libre
	bitmap->Clear(index);
}


int FrameProvider::NumAvailFrame()
{
	// retourne le nombre de cadres libres
	return bitmap->NumClear();
}

#endif

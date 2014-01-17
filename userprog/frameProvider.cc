#ifdef step4
#include "frameProvider.h"
#include "machine.h"
#include "system.h"


FrameProvider::FrameProvider ()
{
	bitmap = new BitMap(NumPhysPages);
	s_frames = new Semaphore("frames", 1);
}

int FrameProvider::GetEmptyFrame()
{
	int indexFrame;
	s_frames->P();
	if ((indexFrame = bitmap->Find ()) == -1)
	{
		DEBUG(',', "Plus de frames physiques disponibles\n");
		s_frames->V();
		return -1;
	}
	// initialisation du cadre à 0
	bzero (&(machine->mainMemory[indexFrame*PageSize]), PageSize);
	// indique que la page d’index “indexFrame” est occupee
	bitmap->Mark(indexFrame);
	s_frames->V();
	return indexFrame;
}

bool FrameProvider::ReleaseFrame(int indexFrame)
{
	bool return_value;
	s_frames->P();
	if(!bitmap->Test(indexFrame))
	{
		DEBUG(',', "Echec de liberation de la frame physique %i non allouee\n", indexFrame);
		ASSERT(FALSE);
		return_value = false;
	}
	else
	{
		DEBUG(',', "liberation de la frame physique %i\n", indexFrame);
		// indique que la page est libre
		bitmap->Clear(indexFrame);
		return_value = true;
	}
	s_frames->V();
	return return_value;
}

int FrameProvider::NumAvailFrame()
{
	// retourne le nombre de cadres libres
	return bitmap->NumClear();
}

int FrameProvider::verifyFrame(int indexFrame)
{
	return bitmap->Test(indexFrame);
}

#endif

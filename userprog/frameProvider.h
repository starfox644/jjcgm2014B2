#ifndef _____frameProvider___
#define _____frameProvider___

#ifdef step4
#include "bitmap.h"
#include "machine.h"
#include "system.h"

class FrameProvider
{
  public:
    FrameProvider ();	    // create a frame provider
    int GetEmptyFrame();  	// renvoie l’adresse physique du premier cadre libre (apres l’avoir initialise) ou -1 s’il n’y en a plus
    void ReleaseFrame(int addrFrame);  // libère la page d’adresse addrFrame
    int NumAvailFrame();	    // renvoie le nombre de cadres disponibles

  private:
    BitMap *bitmap;
};
#endif
#endif

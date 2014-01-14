#ifndef _____frameProvider___
#define _____frameProvider___

#ifdef step4
#include "bitmap.h"

class FrameProvider
{
  public:
    FrameProvider ();	    // create a frame provider
    int GetEmptyFrame();  	// renvoie le numero du premier cadre libre (apres l’avoir initialise) ou -1 s’il n’y en a plus
    void ReleaseFrame(int indexFrame);  // libère la page de numero indexFrame
    int NumAvailFrame();	    // renvoie le nombre de cadres disponibles

  private:
    BitMap *bitmap;
};
#endif
#endif

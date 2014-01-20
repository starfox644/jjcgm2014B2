#ifndef _____frameProvider___
#define _____frameProvider___

#ifdef step4
#include "bitmap.h"
class Semaphore;

class FrameProvider
{
  public:
    FrameProvider ();	    // create a frame provider
    ~FrameProvider ();	    // destroy a frame provider
    int GetEmptyFrame();  	// renvoie le numero du premier cadre libre (apres l’avoir initialise) ou -1 s’il n’y en a plus
    // libère la page de numero indexFrame, retourne false si la page n'a pas ete allouee
    bool ReleaseFrame(int indexFrame);
    int NumAvailFrame();	    // renvoie le nombre de cadres disponibles
    int verifyFrame(int indexFrame);	// verifie si la frame de numero indexFrame est allouee

  private:
    Semaphore* s_frames;
    BitMap *bitmap;
};
#endif
#endif

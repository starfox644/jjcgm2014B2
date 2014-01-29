// filesys.h 
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system. 
//	The "STUB" version just re-defines the Nachos file system 
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.  This is provided in case the
//	multiprogramming and virtual memory assignments (which make use
//	of the file system) are done before the file system assignment.
//
//	The other version is a "real" file system, built on top of 
//	a disk simulator.  The disk is simulated using the native UNIX 
//	file system (in a file named "DISK"). 
//
//	In the "real" implementation, there are two key data structures used 
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.  
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized. 
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "openfile.h"

#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
class FileSystem {
  public:
    FileSystem(bool format) {}

    bool Create(const char *name, int initialSize) { 
	int fileDescriptor = OpenForWrite(name);

	if (fileDescriptor == -1) return FALSE;
	Close(fileDescriptor); 
	return TRUE; 
	}

    OpenFile* Open(char *name) {
	  int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  return new OpenFile(fileDescriptor);
      }

    bool Remove(char *name) { return Unlink(name) == 0; }

};

#else // FILESYS

class FileSystem {
  public:
    FileSystem(bool format);		// Initialize the file system.
					// Must be called *after* "synchDisk" 
					// has been initialized.
    					// If "format", there is nothing on
					// the disk, so initialize the directory
    					// and the bitmap of free blocks.

    bool Create(const char *path, int initialSize);
					// Create a file (UNIX creat)

#ifdef CHANGED

	/**
	 * Deplacement dans le repertoire dont le chemin est passe en argument
	 * Renvoie false si le chemin est invalide, true si le deplacement est possible
	 */
    bool cd(char* path);

    const char* pwd();

    /**
     *  Cree un repertoire vide de nom 'name' dans le repertoire courant
     */
    bool CreateDir(const char *path);

    /*
     * Supprime le repertoire dont le chemin est passe en parametre
     * Renvoie false si le fichier associe au chemin n'est pas un repertoire
     * si ce repertoire n'est pas vide ou que le chemin n'est pas valide
     * sinon la suppression est realisee et la fonction renvoie true
     */
    bool RemoveDirEmpty(char *path);

    /**
     * 	Supprime le fichier dont le chemin est passe en parametre
     * 	renvoie false si le chemin n'est pas valide ou que
     * 	le fichier associe est un repertoire
     */
    bool RemoveFile(char* path);

    bool RemoveDir(char *path);

    /**
     *  Renvoie le numero de secteur du fichier correspondant au chemin
     *  passe en parametre
     *  Si le chemin est invalide, la fonction renvoie -1
     */
    int getSector(const char* path);

    void getLastDirectory(const char* path, char** name, char** subPath);

    /**
     * 	Renvoie vrai si chemin passe en parametre est associe a un fichier
     */
    bool pathExist(const char* path);

    /**
     * 	Decoupe le chemin passe en parametre en une liste composee
     * 	des repertoires du chemin et du dernier fichier
     * 	Si le chemin commence par '/' il est place en premier dans la liste
     * 	Sinon la liste debute par le premier repertoire du chemin
     * 	L'entier pointe par nbDir contient le nombre d'entrees de la liste apres
     * 	l'appel a la fonction
     *	La fonction alloue dynamiquement la liste et la renvoie, elle doit etre
     *	liberee par la suite.
     *	Si un '/' est place a la fin du chemin, il est retire
     *	Si le chemin n'est pas valide, la fonction renvoie NULL
     */
    char** cutPath(const char* path, int* nbDir);

    /**
     * Indique si le chemin passe en parametre est legal
     * Un chemin est illegal si :
     *  - Il contient d'autres caracteres que . / _ - ou (a-z, A-Z, 0-9)
     *  - il contient "//"
     *  - un nom est plus long que la limite
     *  - il est vide
     */
    int isLegalPath(const char* path);
#endif

    OpenFile* Open(const char *path); 	// Open a file (UNIX open)

#ifndef CHANGED
    bool Remove(const char *path); 	// Delete a file (UNIX unlink)
#endif

    void List();			// List all the files in the file system

    void Print();			// List all the files and their contents

  private:
   OpenFile* freeMapFile;		// Bit map of free disk blocks,
					// represented as a file
   OpenFile* directoryFile;		// "Root" directory -- list of 
					// file names, represented as a file
#ifdef CHANGED
   OpenFile* currentDirFile;
   char currentDirName[9];

   bool Remove(const char *path); 	// Delete a file (UNIX unlink)

   void DeleteFile(OpenFile* openfile);

#endif //CHANGED
};

#endif // FILESYS

#endif // FS_H

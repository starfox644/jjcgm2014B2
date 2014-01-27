// directory.cc 
//	Routines to manage a directory of file names.
//
//	The directory is a table of fixed length entries; each
//	entry represents a single file, and contains the file name,
//	and the location of the file header on disk.  The fixed size
//	of each directory entry means that we have the restriction
//	of a fixed maximum size for file names.
//
//	The constructor initializes an empty directory of a certain size;
//	we use ReadFrom/WriteBack to fetch the contents of the directory
//	from disk, and to write back any modifications back to disk.
//
//	Also, this implementation has the restriction that the size
//	of the directory cannot expand.  In other words, once all the
//	entries in the directory are used, no more files can be created.
//	Fixing this is one of the parts to the assignment.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "utility.h"
#include "filehdr.h"
#include "directory.h"
#ifdef CHANGED
#include "system.h"
#endif

//----------------------------------------------------------------------
// Directory::Directory
// 	Initialize a directory; initially, the directory is completely
//	empty.  If the disk is being formatted, an empty directory
//	is all we need, but otherwise, we need to call FetchFrom in order
//	to initialize it from disk.
//
//	"size" is the number of entries in the directory
//----------------------------------------------------------------------

Directory::Directory(int size)
{
    table = new DirectoryEntry[size];
    tableSize = size;
    for (int i = 0; i < tableSize; i++)
	table[i].inUse = FALSE;
}

//----------------------------------------------------------------------
// Directory::~Directory
// 	De-allocate directory data structure.
//----------------------------------------------------------------------

Directory::~Directory()
{ 
    delete [] table;
} 

//----------------------------------------------------------------------
// Directory::FetchFrom
// 	Read the contents of the directory from disk.
//
//	"file" -- file containing the directory contents
//----------------------------------------------------------------------

void
Directory::FetchFrom(OpenFile *file)
{
    (void) file->ReadAt((char *)table, tableSize * sizeof(DirectoryEntry), 0);
#ifdef CHANGED
    selfDir = table[0];
    parentDir = table[1];
#endif
}

//----------------------------------------------------------------------
// Directory::WriteBack
// 	Write any modifications to the directory back to disk
//
//	"file" -- file to contain the new directory contents
//----------------------------------------------------------------------

void
Directory::WriteBack(OpenFile *file)
{
#ifdef CHANGED
	table[0] = selfDir;
	table[1] = parentDir;
#endif
    (void) file->WriteAt((char *)table, tableSize * sizeof(DirectoryEntry), 0);
}

//----------------------------------------------------------------------
// Directory::FindIndex
// 	Look up file name in directory, and return its location in the table of
//	directory entries.  Return -1 if the name isn't in the directory.
//
//	"name" -- the file name to look up
//----------------------------------------------------------------------

int
Directory::FindIndex(const char *name)
{
	/*printf("Contenu : \n");
	for (int i = 0; i < tableSize; i++)
		printf("table[%d].name = %s", i, table[i].name);*/
    for (int i = 0; i < tableSize; i++)
        if (table[i].inUse && !strncmp(table[i].name, name, FileNameMaxLen))
	    return i;
    return -1;		// name not in directory
}

//----------------------------------------------------------------------
// Directory::Find
// 	Look up file name in directory, and return the disk sector number
//	where the file's header is stored. Return -1 if the name isn't 
//	in the directory.
//
//	"name" -- the file name to look up
//----------------------------------------------------------------------

int
Directory::Find(const char *name)
{
    int i = FindIndex(name);
    if (i != -1)
	return table[i].sector;
    return -1;
}

//----------------------------------------------------------------------
// Directory::Add
// 	Add a file into the directory.  Return TRUE if successful;
//	return FALSE if the file name is already in the directory, or if
//	the directory is completely full, and has no more space for
//	additional file names.
//
//	"name" -- the name of the file being added
//	"newSector" -- the disk sector containing the added file's header
//----------------------------------------------------------------------

bool
Directory::Add(const char *name, int newSector)
{ 
    if (FindIndex(name) != -1)
	return FALSE;

    for (int i = 2; i < tableSize; i++)
        if (!table[i].inUse) {
            table[i].inUse = TRUE;
            strncpy(table[i].name, name, FileNameMaxLen); 
            table[i].sector = newSector;
        return TRUE;
	}
    return FALSE;	// no space.  Fix when we have extensible files.
}

//----------------------------------------------------------------------
// Directory::Remove
// 	Remove a file name from the directory.  Return TRUE if successful;
//	return FALSE if the file isn't in the directory. 
//
//	"name" -- the file name to be removed
//----------------------------------------------------------------------

bool
Directory::Remove(const char *name)
{ 
    int i = FindIndex(name);

    if (i == -1)
	return FALSE; 		// name not in directory
    table[i].inUse = FALSE;
    return TRUE;	
}

//----------------------------------------------------------------------
// Directory::List
// 	List all the file names in the directory. 
//----------------------------------------------------------------------

void
Directory::List()
{
#ifdef CHANGED
	int nbElem = 0;

	for (int i = 0; i < tableSize; i++)
		if (table[i].inUse) {
			printf("%s - %s\n",table[i].isDirectory ? "d" : "f", table[i].name);
			nbElem++;
		}
	printf("%d elements\n", nbElem);
#else
	for (int i = 0; i < tableSize; i++)
		if (table[i].inUse)
			printf("%s\n", table[i].name);
#endif
}

//----------------------------------------------------------------------
// Directory::Print
// 	List all the file names in the directory, their FileHeader locations,
//	and the contents of each file.  For debugging.
//----------------------------------------------------------------------

void
Directory::Print()
{ 
    FileHeader *hdr = new FileHeader;

    printf("Directory contents:\n");
    for (int i = 0; i < tableSize; i++)
	if (table[i].inUse)
	{
	    printf("Name: %s, Sector: %d\n", table[i].name, table[i].sector);
	    hdr->FetchFrom(table[i].sector);
	    hdr->Print();
	}
    printf("\n");
    delete hdr;
}

#ifdef CHANGED
    void Directory::setSelfDir(int sector)
    {
    	selfDir.inUse = true;
    	selfDir.sector = sector;
    	strcpy(selfDir.name, ".");
    	selfDir.isDirectory = true;
    }
    void Directory::setParentDir(int sector)
    {
    	parentDir.inUse = true;
    	parentDir.sector = sector;
    	strcpy(parentDir.name, "..");
    	parentDir.isDirectory = true;
    }

    DirectoryEntry Directory::getSelfDir()
    {
    	return selfDir;
    }
    DirectoryEntry Directory::getParentDir()
    {
    	return parentDir;
    }


    bool Directory::isDirectory(char *name)
    {
    	int i = FindIndex(name);
    	if (i == -1)
    		return false; 		// name not in directory
    	else
    		return table[i].isDirectory;
    }


    void Directory::setIsDirectory(const char *name, bool b)
    {
    	int i = FindIndex(name);
    	if (i != -1)
    		 table[i].isDirectory = b;
    }

    /**
     * Fonction permettant de savoir si le repertoire
     * dont le nom est passe en parametre est vide ou non
     */
    /*bool Directory::isEmpty(char *name)
    {
    	Directory* dir = new Directory(tableSize);
    	OpenFile* openFile = NULL;
    	int i = FindIndex(name);
    	int j = 2;

    	if (i == -1)
    	{
    		delete dir;
    		printf(" %s introuvable\n", name);
    		return false; 		// name not in directory
    	}
    	else
    	{
	    	openFile = fileSystem->Open(name);
	    	ASSERT(openFile != NULL);
	    	dir->FetchFrom(openFile);
	    	// verification qu'aucun fichier n'est present dans le repertoire
	        while (j < dir->tableSize && !dir->table[j].inUse)
	        {
	        	printf("verif inUse de %s\n", dir->table[j].name);
	        	j++;
	        }
	        delete openFile;
	        printf("return %d == %d\n", j, dir->tableSize);
	        return (j == dir->tableSize);
    	}
    }
}*/

    char* Directory::findName(int sector)
    {
        for (int i = 0; i < tableSize; i++)
            if (table[i].inUse && sector == table[i].sector)
    	    return table[i].name;
        return NULL;		// sector not in directory
    }

    /**
	 * Fonction permettant de savoir si le repertoire
	 * dont le nom est passe en parametre est vide ou non
	 */
	bool Directory::isEmpty(OpenFile* openFile)
	{
		int j = 2;
    	Directory* dir = new Directory(tableSize);
		dir->FetchFrom(openFile);
		// verification qu'aucun fichier n'est present dans le repertoire
		while (j < dir->tableSize && !dir->table[j].inUse)
		{
			j++;
		}
		delete dir;
		return (j == tableSize);
	}
#endif

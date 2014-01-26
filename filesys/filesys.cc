// filesys.cc 
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk 
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them 
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "directory.h"
#include "filehdr.h"
#include "filesys.h"

// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known 
// sectors, so that they can be located on boot-up.
#define FreeMapSector 		0
#define DirectorySector 	1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number 
// of files that can be loaded onto the disk.
#define FreeMapFileSize 	(NumSectors / BitsInByte)
#define NumDirEntries 		10
#define DirectoryFileSize 	(sizeof(DirectoryEntry) * NumDirEntries)

//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).  
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{ 
	DEBUG('f', "Initializing the file system.\n");
	if (format) {
		BitMap *freeMap = new BitMap(NumSectors);
		Directory *directory = new Directory(NumDirEntries);
#ifdef CHANGED
		directory->setSelfDir(DirectorySector);
		directory->setParentDir(-1);
#endif
		FileHeader *mapHdr = new FileHeader;
		FileHeader *dirHdr = new FileHeader;

		DEBUG('f', "Formatting the file system.\n");

		// First, allocate space for FileHeaders for the directory and bitmap
		// (make sure no one else grabs these!)
		freeMap->Mark(FreeMapSector);
		freeMap->Mark(DirectorySector);

		// Second, allocate space for the data blocks containing the contents
		// of the directory and bitmap files.  There better be enough space!

		ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
		ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));

		// Flush the bitmap and directory FileHeaders back to disk
		// We need to do this before we can "Open" the file, since open
		// reads the file header off of disk (and currently the disk has garbage
		// on it!).

		DEBUG('f', "Writing headers back to disk.\n");
		mapHdr->WriteBack(FreeMapSector);
		dirHdr->WriteBack(DirectorySector);

		// OK to open the bitmap and directory files now
		// The file system operations assume these two files are left open
		// while Nachos is running.

		freeMapFile = new OpenFile(FreeMapSector);
		directoryFile = new OpenFile(DirectorySector);

		// Once we have the files "open", we can write the initial version
		// of each file back to disk.  The directory at this point is completely
		// empty; but the bitmap has been changed to reflect the fact that
		// sectors on the disk have been allocated for the file headers and
		// to hold the file data for the directory and bitmap.

		DEBUG('f', "Writing bitmap and directory back to disk.\n");
		freeMap->WriteBack(freeMapFile);	 // flush changes to disk
		directory->WriteBack(directoryFile);

		if (DebugIsEnabled('f')) {
			freeMap->Print();
			directory->Print();

			delete freeMap;
			delete directory;
			delete mapHdr;
			delete dirHdr;
		}
	} else {
		// if we are not formatting the disk, just open the files representing
		// the bitmap and directory; these are left open while Nachos is running
		freeMapFile = new OpenFile(FreeMapSector);
		directoryFile = new OpenFile(DirectorySector);
#ifdef CHANGED
		currentDirFile = directoryFile;
		strcpy(currentDirName, "/");
#endif
	}
}

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk 
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file 
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool
FileSystem::Create(const char *path, int initialSize)
{
	Directory *directory;
	BitMap *freeMap;
	FileHeader *hdr;
	int sector;
	bool success;
#ifdef CHANGED
	OpenFile *dirFile;
	char* name;
	char* subpath;
	getLastDirectory(path, &name, &subpath);
	if(name == NULL)
	{
		return false;
	}
	if(subpath == NULL)
	{
		dirFile = currentDirFile;
	}
	else
	{
		sector = getSector(subpath);
		if(sector == -1)
		{
			delete subpath;
			delete name;
			return false;
		}
		dirFile = new OpenFile(sector);
		delete subpath;
	}
#endif

	DEBUG('f', "Creating file %s, size %d\n", name, initialSize);

	directory = new Directory(NumDirEntries);
#ifdef CHANGED
	directory->FetchFrom(dirFile);
#else
	directory->FetchFrom(directoryFile);
#endif

	if (directory->Find(name) != -1)
		success = FALSE;			// file is already in directory
	else {
		freeMap = new BitMap(NumSectors);
		freeMap->FetchFrom(freeMapFile);
		sector = freeMap->Find();	// find a sector to hold the file header
		if (sector == -1)
			success = FALSE;		// no free block for file header
		else if (!directory->Add(name, sector))
			success = FALSE;	// no space in directory
		else {
			hdr = new FileHeader;
			if (!hdr->Allocate(freeMap, initialSize))
				success = FALSE;	// no space on disk for data
			else {
				success = TRUE;
				// everthing worked, flush all changes back to disk
				hdr->WriteBack(sector);
#ifdef CHANGED
				directory->WriteBack(currentDirFile);
#else
				directory->WriteBack(directoryFile);
#endif
				freeMap->WriteBack(freeMapFile);
			}
			delete hdr;
		}
		delete freeMap;
	}
	delete directory;
	return success;
}

#ifdef CHANGED
bool FileSystem::CreateDir(const char *name)
{
	OpenFile* openFile;
	Directory* newDir;
	Directory* currentDir;
	DirectoryEntry currentDirEntry;
	int currentSector;
	int newSector;
	// creation d'un fichier pour stocker le repertoire
	if(Create(name, DirectoryFileSize))
	{
		// ouverture du fichier cree
		openFile = Open(name);
		ASSERT(openFile != NULL);
		// lecture du repertoire courant
		currentDir = new Directory(NumDirEntries);
		currentDir->FetchFrom(currentDirFile);
		// modification du booleen isDirectory
		currentDir->setIsDirectory(name, true);
		currentDir->WriteBack(currentDirFile);
		// creation du nouveau repertoire
		newDir = new Directory(NumDirEntries);
		// recuperation du numero de secteur du nouveau repertoire
		newSector = currentDir->Find(name);
		newDir->setSelfDir(newSector);
		// recuperation de l'entree du repertoire courant
		currentDirEntry = currentDir->getSelfDir();
		currentSector = currentDirEntry.sector;
		newDir->setParentDir(currentSector);
		// ecriture du nouveau repertoire dans le fichier attribue
		newDir->WriteBack(openFile);
		delete currentDir;
		delete newDir;
		delete openFile;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Deplacement dans le repertoire dont le chemin est passe en argument
 */
bool FileSystem::cd(char* path)
{
	char** path_dir;
	int i = 0;
	int nbDir, sector;
	Directory* dir = new Directory(NumDirEntries);
	OpenFile* openFile = NULL;
	// verification que le chemin existe
	if (pathExist(path))
	{
		// recuperation du chemin decoupe
		path_dir = cutPath(path, &nbDir);
		// verification que le chemin existe
		if (path_dir != NULL)
		{
			if (nbDir >= 2)
			{
				// parcours des repertoires du path
				while ( i < nbDir-1)
				{
					// si le path commence par /
					//on modifie le repertoire courant
					if (!strcmp(path_dir[i], "/"))
					{
						if(currentDirFile != directoryFile)
							delete currentDirFile;
						openFile = directoryFile;
						currentDirFile = openFile;
					}
					else
					{
						openFile = currentDirFile;
						dir->FetchFrom(openFile);
						sector = dir->Find(path_dir[i]);
						if (sector >= 0)
							openFile = new OpenFile(sector);
						else
							ASSERT(false);
						ASSERT(openFile != NULL);
						if(currentDirFile != directoryFile)
							delete currentDirFile;
						// modification du repertoire courant
						currentDirFile = openFile;
					}
					i++;
				}
			}
			else
				openFile = currentDirFile;
			dir->FetchFrom(openFile);
			// si le dernier nom est un repertoire
			if (dir->isDirectory(path_dir[nbDir-1]))
			{
				if(currentDirFile != directoryFile)
					delete currentDirFile;
				// recherche du secteur du repertoire
				sector = dir->Find(path_dir[nbDir-1]);
				if (sector >= 0)
					openFile = new OpenFile(sector);
				ASSERT(openFile != NULL);
				// modification du repertoire courant
				currentDirFile = openFile;
				strncpy(currentDirName, path_dir[nbDir-1], 9);
				delete dir;
				delete path_dir;
				return true;
			}
			delete path_dir;
		}
	}
	printf("Impossible d'acceder a : %s\n", path);
	delete dir;
	return false;
}

/**
 * Fonction permettant de supprimer le repertoire
 * dont le chemin d'acces est passe en parametre
 */
bool FileSystem::RemoveDirEmpty(char *path)
{
	char** path_dir;
	int nbDir;
	OpenFile* openFile = NULL;
	Directory* dir = new Directory(NumDirEntries);
	//if ((sector = getSector(path)) != -1)
	if((openFile = Open(path)) != NULL)
	{
		path_dir = cutPath(path, &nbDir);
		dir->FetchFrom(openFile);
		printf("verif repertoire %s\n", path_dir[nbDir-1]);
		// si le dernier nom est un repertoire
		if (dir->getSelfDir().isDirectory)
		{
			printf("C'est un repertoire \n");
			// suppression du repertoire s'il est vide
			if (dir->isEmpty(openFile))
			{
				printf("Le repertoire est vide \n");
				//Remove(path_dir[nbDir-1]);
				Remove(path);
			}
			else
				printf("Le repertoire n'est pas vide : suppression impossible\n");
			delete dir;
			delete path_dir;
			if(currentDirFile != directoryFile)
				delete openFile;
			return true;
		}
	}
	else
		printf("Open fail\n");
	return false;
}

bool FileSystem::RemoveDir(char *path)
{
	return false;
}

char* FileSystem::pwd()
{
	return currentDirName;
}

/**
 *  Return the sector of the file associated to the given path
 *  or -1 if the path is not valable
 */
int FileSystem::getSector(const char* path)
{
	int nbDir;
	int i;
	int curSector;
	char** pathList;
	OpenFile* fileActu;
	Directory* dir = new Directory(NumDirEntries);
	// decoupage du chemin en une liste
	pathList = cutPath(path, &nbDir);
	if(pathList != NULL)
	{
		if(!strcmp(pathList[0], "/"))
		{
			if(nbDir == 1)
			{
				// juste la racine : vrai
				delete pathList;
				delete dir;
				return DirectorySector;
			}
			else
			{
				// chemin absolu : on commence a la racine
				curSector = DirectorySector;
				// la racine n'est pas comptee
				i = 1;
			}
		}
		else
		{
			// chemin relatif : on commence au repertoire courant
			dir->FetchFrom(currentDirFile);
			curSector = (dir->getSelfDir()).sector;
			i = 0;
		}
		// ouverture du fichier du premier repertoire
		fileActu = new OpenFile(curSector);
		while(i < nbDir - 1 && curSector != -1)
		{
			// lecture du repertoire actuel
			dir->FetchFrom(fileActu);
			delete fileActu;
			// le nom suivant doit etre un repertoire
			if(dir->isDirectory(pathList[i]))
			{
				// recherche du repertoire suivant
				curSector = dir->Find(pathList[i]);
				if(curSector != -1)
				{
					fileActu = new OpenFile(curSector);
					i++;
				}
			}
			else
			{
				printf("le fichier %s n'est pas un repertoire\n", pathList[i]);
				delete pathList;
				delete dir;
				return -1;
			}
		}
		if(curSector == -1)
		{
			printf("le repertoire %s est introuvable\n", pathList[i]);
			// erreur : un dossier du path est introuvable
			delete pathList;
			delete dir;
			return -1;
		}
		else
		{
			// lecture du dernier repertoire
			dir->FetchFrom(fileActu);
			delete fileActu;
			// recherche du dernier fichier
			curSector = dir->Find(pathList[i]);
			delete pathList;
			delete dir;
			return curSector;
		}
	}
	else
	{
		delete pathList;
		delete dir;
		return -1;
	}
}

/*
 * Return true if the path already exists
 */
bool FileSystem::pathExist(const char* path)
{
	return (getSector(path) != -1);
}



/**
 * Tell if a path is legal or not
 * If it is legal, it returns the numbers of levels in the path,
 * else it returns 0
 * The path is considered illegal if :
 *  - it contains characters others than . / _ - and alphanum (a-z, A-Z, 0-9)
 *  - there are two null names "//"
 *  - one name is longer than limit
 *  - it is void
 */
int FileSystem::isLegalPath(const char* path) {
	int i, legal, level, nameLength;
	char c, prev;

	legal = 1;
	i = 0;
	prev = 0;
	level = 1;
	nameLength = 0;

	// if the path is void
	if (strlen(path) == 0)
		legal = 0;

	// while we are not at the end of the path string
	while (path[i] != '\0' && legal) {
		c = path[i];

		// if we find a /, we increment the number of levels
		// and we reset the name length
		if (c == '/') {
			level++;
			nameLength = 0;
		} else {
			// we increase name length
			nameLength++;

			// if it's too big, the path is not legal
			if (nameLength > FileNameMaxLen)
				legal = 0;
		}

		// if there are two /, not legal
		if (c == '/' && prev == '/')
			legal = 0;

		// if c is not a alphanum, or . / - _
		if ((c < 'a' || 'z' < c) && (c < 'A' || 'Z' < c) && (c < '0' || '9' < c)
				&& c != '/' && c != '.' && c != '-' && c != '_')
			legal = 0;

		// we keep the previous character in memory
		prev = c;
		i++;
	}

	// if there is a / at the end, we decrease the number level
	if (path[i - 1] == '/') {
		level--;
	}

	// if it isn't legal, we return 0, else we return the level
	if (legal == 0)
		return 0;
	else
		return level;
}

/**
 * Cuts a path name into multiple names.
 * Returns an array of names if the path is legal
 *    and nbDir contains the size of the array
 * If the path is illegal, the function returns NULL
 */
char** FileSystem::cutPath(const char* path, int* nbDir) {
	int i, j, k;
	char** result;

	result = NULL;

	*nbDir = isLegalPath(path);

	if (*nbDir == 0) // if illegalPath
		return NULL;

	// we allocate the result
	result = new char*[*nbDir];
	for (i = 0; i < *nbDir; i++)
		result[i] = new char[FileNameMaxLen];

	i = 0; // line in the result
	j = 0; // column in the result
	k = 0; // character in the path

	// if the path beggins with a /, we add it to the result
	if (path[0] == '/') {
		result[0][0] = '/';
		result[0][1] = '\0';
		if(path[1] == '\0')
		{
			// just '/' : return
			return result;
		}
		i++;
		k++;
	}


	// while we are not at the end of the path
	while (path[k] != '\0') {

		// if we find a /, we change path
		if (path[k] == '/') {

			// the case of the last / is treated at the end
			if (path[k + 1] != '\0') {
				result[i][j] = '\0';
				i++;
				j = 0;
			}
		} else { // else we just copy
			result[i][j] = path[k];
			j++;
		}

		k++;
	}

	// don't forget the '\0' at the end of the last string
	result[i][j] = '\0';

	return result;
}

void FileSystem::getLastDirectory(const char* path, char** name, char** subPath)
{
	int i = 0;
	int size = strlen(path);
	if(!isLegalPath(path))
	{
		*name = NULL;
		*subPath = NULL;
		return;
	}
	*name = new char[size];
	if(size > 1 && path[size-1] == '/')
	{
		i = size - 2;
	}
	else
	{
		i = size - 1;
	}
	while(i > 0 && path[i] != '/')
	{
		i--;
	}
	if(i == 0 && path[i] != '/')
	{
		// nom seul : fichier dans le repertoire courant
		strcpy(*name, path);
		*subPath = NULL;
	}
	else if(i == 0 && path[i] == '/')
	{
		*subPath = new char[size];
		*name = NULL;
		strcpy(*subPath, path);
	}
	else
	{
		*subPath = new char[size];
		// on copie le path dans le sous path
		strcpy(*subPath, path);
		// le sous path s'arrete au dernier /
		(*subPath)[i] = '\0';
		// copie du nom du fichier
		strcpy(*name, &path[i+1]);
	}
	if(name != NULL)
	{
		size = strlen(*name);
		if((*name)[size - 1] == '/')
		{
			(*name)[size - 1] = '\0';
		}
	}
}
#endif //CHANGED

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.  
//	To open a file:
//	  Find the location of the file's header, using the directory 
//	  Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(const char *name)
{
#ifdef CHANGED
	int sector = getSector(name);
	if(sector >= 0)
	{
		return new OpenFile(sector);
	}
	else
	{
		return NULL;
	}
#else
	Directory *directory = new Directory(NumDirEntries);
	OpenFile *openFile = NULL;
	int sector;

	DEBUG('f', "Opening file %s\n", name);
	directory->FetchFrom(directoryFile);
	sector = directory->Find(name);
	if (sector >= 0)
		openFile = new OpenFile(sector);	// name was found in directory
	delete directory;
	return openFile;				// return NULL if not found
#endif
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

bool
FileSystem::Remove(const char *path)
{ 
	Directory *directory;
	BitMap *freeMap;
	FileHeader *fileHdr;
	int sector;
#ifdef CHANGED
	OpenFile *dirFile;
	char* name;
	char* subpath;
	getLastDirectory(path, &name, &subpath);
	if(name == NULL)
	{
		return false;
	}
	if(subpath == NULL)
	{
		dirFile = currentDirFile;
	}
	else
	{
		sector = getSector(subpath);
		if(sector == -1)
		{
			delete subpath;
			delete name;
			return false;
		}
		dirFile = new OpenFile(sector);
		delete subpath;
	}
#endif
	directory = new Directory(NumDirEntries);

#ifdef CHANGED
	directory->FetchFrom(dirFile);
#else
	directory->FetchFrom(directoryFile);
#endif
	sector = directory->Find(name);
	if (sector == -1) {
		delete directory;
		return FALSE;			 // file not found
	}
	fileHdr = new FileHeader;
	fileHdr->FetchFrom(sector);

	freeMap = new BitMap(NumSectors);
	freeMap->FetchFrom(freeMapFile);

	fileHdr->Deallocate(freeMap);  		// remove data blocks
	freeMap->Clear(sector);			// remove header block
	directory->Remove(name);

	freeMap->WriteBack(freeMapFile);		// flush to disk
	directory->WriteBack(directoryFile);        // flush to disk
	delete fileHdr;
	delete directory;
	delete freeMap;
	return TRUE;
}

//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------

void
FileSystem::List()
{
	Directory *directory = new Directory(NumDirEntries);
#ifdef CHANGED
	directory->FetchFrom(currentDirFile);
#else
	directory->FetchFrom(directoryFile);
#endif
	directory->List();
	delete directory;
}

//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

void
FileSystem::Print()
{
	FileHeader *bitHdr = new FileHeader;
	FileHeader *dirHdr = new FileHeader;
	BitMap *freeMap = new BitMap(NumSectors);
	Directory *directory = new Directory(NumDirEntries);

	printf("Bit map file header:\n");
	bitHdr->FetchFrom(FreeMapSector);
	bitHdr->Print();

	printf("Directory file header:\n");
	dirHdr->FetchFrom(DirectorySector);
	dirHdr->Print();

	freeMap->FetchFrom(freeMapFile);
	freeMap->Print();

	directory->FetchFrom(directoryFile);
	directory->Print();

	delete bitHdr;
	delete dirHdr;
	delete freeMap;
	delete directory;
} 

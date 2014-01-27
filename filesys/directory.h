// directory.h 
//	Data structures to manage a UNIX-like directory of file names.
// 
//      A directory is a table of pairs: <file name, sector #>,
//	giving the name of each file in the directory, and 
//	where to find its file header (the data structure describing
//	where to find the file's data blocks) on disk.
//
//      We assume mutual exclusion is provided by the caller.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "openfile.h"

#define FileNameMaxLen 		9	// for simplicity, we assume 
// file names are <= 9 characters long

// The following class defines a "directory entry", representing a file
// in the directory.  Each entry gives the name of the file, and where
// the file's header is to be found on disk.
//
// Internal data structures kept public so that Directory operations can
// access them directly.

class DirectoryEntry {
public:
	bool inUse;				// Is this directory entry in use?
	int sector;				// Location on disk to find the
	//   FileHeader for this file
	char name[FileNameMaxLen + 1];	// Text name for file, with +1 for
	// the trailing '\0'
#ifdef CHANGED
	bool isDirectory;		// Is a directory ?
#endif
};

// The following class defines a UNIX-like "directory".  Each entry in
// the directory describes a file, and where to find it on disk.
//
// The directory data structure can be stored in memory, or on disk.
// When it is on disk, it is stored as a regular Nachos file.
//
// The constructor initializes a directory structure in memory; the
// FetchFrom/WriteBack operations shuffle the directory information
// from/to disk. 

class Directory {
public:
	Directory(int size); 		// Initialize an empty directory
	// with space for "size" files
	~Directory();			// De-allocate the directory

	void FetchFrom(OpenFile *file);  	// Init directory contents from disk
	void WriteBack(OpenFile *file);	// Write modifications to
	// directory contents back to disk

	int Find(const char *name);		// Find the sector number of the
	// FileHeader for file: "name"

	bool Add(const char *name, int newSector);  // Add a file name into the directory

	bool Remove(const char *name);	// Remove a file from the directory

	void List();			// Print the names of all the files
	//  in the directory
	void Print();			// Verbose print of the contents
	//  of the directory -- all the file
	//  names and their contents.

#ifdef CHANGED

	/**
	 * 	Set the entry correspondant to this directory with "."
	 * 	sector is the sector number of this directory
	 */
	void setSelfDir(int sector);

	/**
	 * 	Set the entry correspondant to this directory's parent with ".."
	 * 	sector is the sector number of this directory
	 */
	void setParentDir(int sector);

	/**
	 * 	Return the entry "." which correspond to this directory
	 */
	DirectoryEntry getSelfDir();

	/**
	 *  Return the entry ".." which correspond to the parent directory
	 *  For the root this entry has -1 as sector number
	 */
	DirectoryEntry getParentDir();

	/**
	 * 	Return true if the entry 'name' of this directory is a directory
	 * 	If the name is not found in the directory
	 * 	or if it's not a directory, this function returns false
	 */
    bool isDirectory(char *name);

    /**
     * 	Set the boolean isDirectory of a directory entry
     */
    void setIsDirectory(const char *name, bool b);

    /**
     * 	Set the boolean inUse of a directory entry
     */
    void setInUse(int num_sector, bool bUse);

    /**
     * 	Return true if the directory is empty
     */
    bool isEmpty(char *name);

    /**
     *	Return the name of the entry which has the sector number given
     */
    char* findName(int sector);

#endif
private:
	int tableSize;			// Number of directory entries
	DirectoryEntry *table;		// Table of pairs:
	// <file name, file header location>

	int FindIndex(const char *name);	// Find the index into the directory
	//  table corresponding to "name"
#ifdef CHANGED
	DirectoryEntry selfDir;	// entry of this directory
	DirectoryEntry parentDir;	// entry of the parent directory
#endif
};

#endif // DIRECTORY_H

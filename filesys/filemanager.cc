#ifdef step5

#include "filemanager.h"


FileManager* fm = NULL;

extern FileSystem *fileSystem;

OpenFileId do_Open (char *name) {
	OpenFile* file = NULL;
	OpenFileId id;

	DEBUG('f', "Opening file %s\n", name);

	// if a there is no FileManager, we create a new one
	if (fm == NULL)
		fm = new FileManager ();

	// if we can't open a new file, we return -1
	if (fm->getNbOpen () == MAX_FILES_OPEN)
		return -1;

	// we open the file
	file = fileSystem->Open(name);
	if (file == NULL) {
		DEBUG('f', "Error opening file %s\n", name);
		return -1;
	}

	id = fm->insertFile (file);
	// in case of error
	if (id == -1) {
		DEBUG('f', "Cannot insert new file in table : aborting\n", name);
		delete file;
		return -1;
	}

	fm->increaseNbOpen ();

	return id;
}

void do_Close (OpenFileId id) {
	DEBUG('f', "Closing file %d\n", id);

	fm->removeFile(id);

	fm->decreaseNbOpen ();
	if (fm->getNbOpen () == 0)
		delete fm;
}
/*
Function to use

OpenFile *
FileSystem::Open(const char *name)
*/


/*
void do_Write (char *buffer, int size, OpenFileId id) {

}

int do_Read (char *buffer, int size, OpenFileId id) {

}
*/



FileManager::FileManager() {
	int i;

	DEBUG('f', "Allocation of new FileManager\n");

	nbOpen = 0;
	table = new OpenFile*[MAX_FILES_OPEN];

	for (i = 0; i < MAX_FILES_OPEN; i++) {
		table[i] = NULL;
	}
}

FileManager::~FileManager() {
	DEBUG('f', "Destruction of FileManager\n");
	delete table;
}

OpenFileId FileManager::insertFile(OpenFile* file) {
	int i;

	DEBUG('f', "Insertion of new file in table\n");

	i = 0;
	while (i < MAX_FILES_OPEN && table[i] != NULL)
		i++;

	if (i == MAX_FILES_OPEN)
		i = -1;
	else
		table[i] = file;

	return i;
}

void FileManager::removeFile(OpenFileId id) {
	DEBUG('f', "Remove file fromtable\n");
	delete table[id];
	table[id] = NULL;
}

void FileManager::increaseNbOpen() {
	nbOpen++;
}

void FileManager::decreaseNbOpen() {
	nbOpen--;
}

int FileManager::getNbOpen() {
	return nbOpen;
}


#endif

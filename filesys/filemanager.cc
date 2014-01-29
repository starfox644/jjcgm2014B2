//#ifdef step5

#include "filemanager.h"
#include "system.h"

FileManager* fm = NULL;

extern FileSystem *fileSystem;

OpenFileId do_Open (char *name) {
	OpenFile* file = NULL;
	OpenFileId id;
	OpenFileId processId;
	DEBUG('f', "Opening file %s\n", name);

	// if a there is no FileManager, we create a new one
	if (fm == NULL)
		fm = new FileManager ();

	// if we can't open a new file, we return -1
	if (fm->getNbOpen () == MAX_FILES_OPEN)
		return -1;

	if(!fileSystem->pathExist(name))
	{
		printf("path non existant\n");
		// if we just created the FileManager, we delete it to avoid memory leak
		if (fm->getNbOpen() == 0)
		{
			delete fm;
			fm = NULL;
		}
		return -1;
	}
	// verify if the process can open a file
	if(!currentProcess->canAddFile())
	{
		// if we just created the FileManager, we delete it to avoid memory leak
		if (fm->getNbOpen() == 0)
		{
			delete fm;
			fm = NULL;
		}
		return -1;
	}
	// we open the file
	file = fileSystem->Open(name);
	if (file == NULL) {
		DEBUG('f', "Error opening file %s\n", name);
		// if we just created the FileManager, we delete it to avoid memory leak
		if (fm->getNbOpen() == 0)
		{
			delete fm;
			fm = NULL;
		}
		return -1;
	}
	// verify if the process has already open this file
	if(fm->hasProcessSector(file->getSector()))
	{
		if (fm->getNbOpen() == 0)
		{
			delete fm;
			fm = NULL;
		}
		delete file;
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
	// convert file id into process file id
	processId = currentProcess->addOpenFile(id);
	return processId;
}

int do_Close (OpenFileId processFileId) {
	int res = 0;
	int id = currentProcess->getOpenFileId(processFileId);
	if(id == -1)
	{
		DEBUG('f', "Error : file does not exists\n", id);
		return -1;
	}
	// if we try to close a non existent file
	if (fm == NULL || fm->getFile(id) == NULL)
	{
		DEBUG('f', "Error : file does not exists\n", id);
		ASSERT(false);
		res = -1;
	}
	else
	{
		DEBUG('f', "Closing file %d\n", id);
		if(!currentProcess->removeOpenFile(processFileId))
		{
			ASSERT(false);
		}
		fm->removeFile(id);

		fm->decreaseNbOpen ();
		if (fm->getNbOpen () == 0)
		{
			delete fm;
			fm = NULL;
		}
	}

	return res;
}

int do_Write (int buffer, int size, OpenFileId id)
{
	int i = 0;
	int tmp;
	char c;
	bool noError = true;
	OpenFile* openfile = fm->getFile(id);
	if(openfile == NULL)
	{
		return -1;
	}
	while(i < size && noError)
	{
		noError = machine->ReadMem(buffer+(i*sizeof(char)), sizeof(char), &tmp);
		if(noError)
		{
			c = (char)tmp;
			noError = (openfile->Write(&c, sizeof(char)) == sizeof(char));
			if(noError)
				i++;
		}
	}
	return i;
}

int do_Read (int buffer, int size, OpenFileId id)
{
	int i = 0;
	unsigned int tmp;
	char c;
	bool noError = true;
	OpenFile* openfile = fm->getFile(id);
	if(openfile == NULL)
	{
		return -1;
	}
	while(i < size && noError)
	{
		noError = (openfile->Read(&c, sizeof(char)) == sizeof(char));
		if(noError)
		{
			tmp = (unsigned int)c;
			noError = machine->WriteMem(buffer+(i*sizeof(char)), sizeof(char), tmp);
			if(noError)
			{
				i++;
			}
		}
	}
	return i;
}

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

bool FileManager::isSectorOpen(int sector)
{
	bool isOpen = false;
	int i = 0;
	while(i < MAX_FILES_OPEN && !isOpen)
	{
		if(table[i] != NULL)
		{
			isOpen = (table[i]->getSector() == sector);
		}
	}
	return isOpen;
}

bool FileManager::hasProcessSector(int sector)
{
	bool isOpen = false;
	OpenFile *openfile;
	int id;
	int i = 0;
	while(i < NB_FILES_PROCESS && !isOpen)
	{
		// verification de chaque indice de fichier du processus
		id = currentProcess->getOpenFileId(i);
		if(id != -1)
		{
			openfile = getFile(i);
			// le processus indique qu'il a le fichier, il doit etre present
			ASSERT(openfile != NULL);
			isOpen = (openfile->getSector() == sector);
		}
		i++;
	}
	return isOpen;
}

OpenFile* FileManager::getFile(OpenFileId id)
{
	if(id < 0 || id >= MAX_FILES_OPEN)
	{
		DEBUG('f', "Invalid open file id, out of bounds\n");
		return NULL;
	}
	else
		return table[id];
}

//#endif

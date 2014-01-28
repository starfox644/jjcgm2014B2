//#ifdef step5
#ifndef ___FILEMANAGER
#define ___FILEMANAGER

#include "openfile.h"
#include "filesys.h"

#define MAX_FILES_OPEN 10

typedef int OpenFileId;


OpenFileId do_Open (char *name);

int do_Write (int buffer, int size, OpenFileId id);

int do_Read (int buffer, int size, OpenFileId id);

int do_Close (OpenFileId id);


class FileManager {

private :
	int nbOpen;
	OpenFile** table;

public :
	FileManager();
	~FileManager();

	OpenFileId insertFile(OpenFile* file);
	void removeFile(OpenFileId id);

	void increaseNbOpen();
	void decreaseNbOpen();
	int getNbOpen();
	OpenFile* getFile(OpenFileId id);
};

#endif

#ifdef step5

#include "openfile.h"
#include "filesys.h"

#define MAX_FILES_OPEN 10

typedef int OpenFileId;


OpenFileId do_Open (char *name);

void do_Write (char *buffer, int size, OpenFileId id);

int do_Read (char *buffer, int size, OpenFileId id);

void do_Close (OpenFileId id);


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
};

#endif

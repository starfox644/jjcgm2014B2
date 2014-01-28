// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "filehdr.h"

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize)
{ 
	numBytes = fileSize;
#ifdef CHANGED
	IndirectTable table;
	int tmp;
	unsigned int numSectorsData = divRoundUp(fileSize, SectorSize);
	numSectors = divRoundUp(numSectorsData, NumDirect);
	if (freeMap->NumClear() < (numSectorsData + numSectors))
	{
		return FALSE;		// not enough space
	}

	for (unsigned int i = 0; i < numSectors ; i++)
	{
		sectorTable[i] = freeMap->Find();
		// allocate all sectors of table
		if(numSectorsData >= NumDirect)
		{
			tmp = numSectorsData;
			table.Allocate(freeMap, NumDirect);
			numSectorsData = tmp;
			numSectorsData -= NumDirect;
		}
		// allocate remaining sectors
		else
		{
			table.Allocate(freeMap, numSectorsData);
		}
		// write back table contents
		table.WriteBack(sectorTable[i]);
	}
	return TRUE;
#else
	numSectors  = divRoundUp(fileSize, SectorSize);
	if (freeMap->NumClear() < numSectors)
		return FALSE;		// not enough space
	for (int i = 0; i < numSectors; i++)
		dataSectors[i] = freeMap->Find();
	return TRUE;
#endif
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void 
FileHeader::Deallocate(BitMap *freeMap)
{
#ifdef CHANGED
	IndirectTable table;
	for (unsigned int i = 0; i < numSectors; i++)
	{
		ASSERT(freeMap->Test((int) sectorTable[i]));  // ought to be marked!
		// read table contents
		table.FetchFrom(sectorTable[i]);
		// deallocate indirection table
		table.Deallocate(freeMap);
		freeMap->Clear((int) sectorTable[i]);
	}
#else
	for (int i = 0; i < numSectors; i++) {
		ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
		freeMap->Clear((int) dataSectors[i]);
	}
#endif
}

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
	synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
	synchDisk->WriteSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
#ifdef CHANGED
	// numero de secteur d'indirection
	IndirectTable table;
	int indirectNum = divRoundDown(offset, NumDirect * SectorSize);
	table.FetchFrom(sectorTable[indirectNum]);
	return table.ByteToSector(offset % (NumDirect * SectorSize));
#else
	return(dataSectors[offset / SectorSize]);
#endif
}

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
	return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print()
{
#ifdef CHANGED
#else
	int i, j, k;
	char *data = new char[SectorSize];

	printf("FileHeader contents.  File size: %d.  File blocks:\n", numBytes);
	for (i = 0; i < numSectors; i++)
		printf("%d ", dataSectors[i]);
	printf("\nFile contents:\n");
	for (i = k = 0; i < numSectors; i++) {
		synchDisk->ReadSector(dataSectors[i], data);
		for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
			if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
				printf("%c", data[j]);
			else
				printf("\\%x", (unsigned char)data[j]);
		}
		printf("\n");
	}
	delete [] data;
#endif
}

#ifdef CHANGED
void IndirectTable::Allocate(BitMap *bitMap, unsigned int nbSectors)
{
	ASSERT(nbSectors <= NumDirect);
	ASSERT((unsigned int)bitMap->NumClear() >= nbSectors);
	numSectors = nbSectors;
	for (int i = 0; i < numSectors ; i++)
	{
		dataSectors[i] = bitMap->Find();
	}
}

void IndirectTable::Deallocate(BitMap *bitMap)
{
	for (int i = 0; i < numSectors; i++) {
		ASSERT(bitMap->Test((int) dataSectors[i]));  // ought to be marked!
		bitMap->Clear((int) dataSectors[i]);
	}
}

// Initialize table from disk
void IndirectTable::FetchFrom(int sectorNumber)
{
	synchDisk->ReadSector(sectorNumber, (char *)this);
}

// Write modifications to table back to disk
void IndirectTable::WriteBack(int sectorNumber)
{
	synchDisk->WriteSector(sectorNumber, (char *)this);
}

int
IndirectTable::ByteToSector(int offset)
{
	return(dataSectors[offset / SectorSize]);
}
#endif

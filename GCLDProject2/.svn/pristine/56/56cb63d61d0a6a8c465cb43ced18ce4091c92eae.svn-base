//#include "preHeader.h"
// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//#include "stdafx.h"
#include "CLimitReadResFile.h"

CLimitReadResFile::CLimitReadResFile(IReadResFile* alreadyOpenedFile, long areaSize, const char* name)
: Filename(name), AreaSize(areaSize), AreaStart(0), AreaEnd(0), File(alreadyOpenedFile), LastPos(0)
{
	#ifdef _DEBUG
	setDebugName("CLimitReadResFile");
	#endif
	File = alreadyOpenedFile->clone();
	init();
}


IReadResFile* CLimitReadResFile::clone() const
{
	IReadResFile* lrf = new CLimitReadResFile(File, AreaSize, Filename.c_str());
	((CLimitReadResFile*)lrf)->AreaEnd = AreaEnd;
	((CLimitReadResFile*)lrf)->AreaStart = AreaStart;
	((CLimitReadResFile*)lrf)->LastPos = LastPos;
	return lrf;
}


void CLimitReadResFile::init()
{
	if (!File)
		return;

	AreaStart = File->getPos();
	AreaEnd = AreaStart + AreaSize;
	File->seek(AreaStart);
	LastPos = AreaStart;
}



CLimitReadResFile::~CLimitReadResFile()
{
	//if (File)
	//	File->drop();
}



//! returns how much was read
S32 CLimitReadResFile::read(void* buffer, U32 sizeToRead)
{
	long pos = File->getPos();

	// Ok, another reader using the same "File" changed our pos, replace it!

	if(LastPos != pos)
	{
		File->seek(LastPos);
		pos = LastPos;
	}

	if (pos >= AreaEnd)
	{
		return 0;
	}

	if (pos + (long)sizeToRead >= AreaEnd)
	{
		sizeToRead = AreaEnd - pos;
	}

	S32 result = File->read(buffer, sizeToRead);
	LastPos += result;
	return result;
}



//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CLimitReadResFile::seek(long finalPos, bool relativeMovement)
{
	const long pos = File->getPos();

	// Adjust the seek to manage multiple CLimitReadResFile
	finalPos += pos - LastPos;

	if (relativeMovement)
	{
		if (LastPos + finalPos > AreaEnd)
			finalPos = AreaEnd - pos;

		LastPos = pos + finalPos;
	}
	else
	{
		finalPos += AreaStart;
		if (finalPos > AreaEnd)
			return false;

		LastPos = finalPos;
	}

	return File->seek(finalPos, relativeMovement);	
}


//! returns size of file
long CLimitReadResFile::getSize() const
{
	return AreaSize;
}



//! returns where in the file we are.
long CLimitReadResFile::getPos() const
{
	return LastPos- AreaStart;
}



//! returns name of file
const char* CLimitReadResFile::getFileName() const
{
	return Filename.c_str();
}

//! return full path
const char* CLimitReadResFile::getFullPath() const
{
	return Filename.c_str();
}


IReadResFile* createLimitReadFile(const char* fileName, IReadResFile* alreadyOpenedFile, long areaSize)
{
	return new CLimitReadResFile(alreadyOpenedFile, areaSize, fileName);
}




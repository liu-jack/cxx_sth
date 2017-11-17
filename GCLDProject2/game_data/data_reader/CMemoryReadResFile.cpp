//#include "preHeader.h"
// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//#include "stdafx.h"
#include "CMemoryReadResFile.h"


CMemoryReadResFile::CMemoryReadResFile()
: Buffer(0), Len(0), Pos(0), Filename("")
{
}

CMemoryReadResFile::CMemoryReadResFile(void* memory, long len, const char* fileName, bool d)
: Buffer(memory), deleteMemWhenDrop(d), Len(len), Pos(0), Filename(fileName)
{
	//if (d)
	//	SharedBuffer.reset((char*)memory);

	#ifdef _DEBUG
	setDebugName("CMemoryReadResFile");
	#endif
}


CMemoryReadResFile::~CMemoryReadResFile()
{
	if(deleteMemWhenDrop)
		delete[] ((char *)Buffer);
}

IReadResFile* CMemoryReadResFile::clone() const
{
	CMemoryReadResFile* mrf = new CMemoryReadResFile();

	mrf->Buffer				 = this->Buffer;
	mrf->deleteMemWhenDrop	 = this->deleteMemWhenDrop;
	mrf->Len				 = this->Len;
	mrf->Pos				= this->Pos;
	mrf->Filename			= this->Filename;
	
	return mrf;
}


void CMemoryReadResFile::set(void* memory, long len, const char* fileName)
{
	Buffer = memory;
	Len = len;
	Pos = 0;
	Filename = fileName;
}

bool CMemoryReadResFile::isValid() const
{
	return Buffer && Len >= 0;
}

//! returns how much was read
S32 CMemoryReadResFile::read(void* buffer, U32 sizeToRead)
{
	S32 amount = static_cast<S32>(sizeToRead);
	if (Pos + amount > Len)
		amount -= Pos + amount - Len;

	if (amount <= 0)
		return 0;

	char* p = (char*)Buffer;
	memcpy(buffer, p + Pos, amount);
	
	Pos += amount;

	return amount;
}


//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CMemoryReadResFile::seek(long finalPos, bool relativeMovement)
{
	if (relativeMovement)
	{
		if (Pos + finalPos > Len)
			return false;

		Pos += finalPos;
	}
	else
	{
		if (finalPos > Len)
			return false;
		
		Pos = finalPos;
	}

	return true;
}


//! returns size of file
long CMemoryReadResFile::getSize() const
{
	return Len;
}


//! returns where in the file we are.
long CMemoryReadResFile::getPos() const
{
	return Pos;
}


//! returns name of file
const char* CMemoryReadResFile::getFileName() const
{
	return Filename.c_str();
}

const char* CMemoryReadResFile::getFullPath() const
{
	return Filename.c_str();
}


IReadResFile* createMemoryReadFile(void* memory, long size, const char* fileName, bool deleteMemoryWhenDropped)
{
	CMemoryReadResFile* file = new CMemoryReadResFile(memory, size, fileName, deleteMemoryWhenDropped);
	return file;
}


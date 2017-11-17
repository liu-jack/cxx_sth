// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_MEMORY_READ_FILE_H_INCLUDED__
#define __C_MEMORY_READ_FILE_H_INCLUDED__

#pragma once

#include "IReadResFile.h"
#include <string>

/*!
Class for reading from memory.
*/
class CMemoryReadResFile : public IReadResFile
{
public:

	//! Constructor
	CMemoryReadResFile();
	CMemoryReadResFile(void* memory, long len, const char* fileName, bool deleteMemoryWhenDropped);

	//! Destructor
	virtual ~CMemoryReadResFile();

	IReadResFile* clone() const;

	//! set the memory, length and filename
	void set(void* memory, long len, const char* fileName);

	//! returns if the file is valid
	bool isValid() const;


	//! returns how much was read
	virtual S32 read(void* buffer, U32 sizeToRead);

	//! changes position in file, returns true if successful
	virtual bool seek(long finalPos, bool relativeMovement = false);

	//! returns size of file
	virtual long getSize() const;

	//! returns where in the file we are.
	virtual long getPos() const;

	//! returns name of file
	virtual const char* getFileName() const;

	//! return complete path
	virtual const char* getFullPath() const;

	virtual void* getBuffer(long* size) {
		if(size) *size = Pos;
		return Buffer;
	}

	virtual bool isAllInMemory() const {
		return true;
	}

private:
	void* Buffer;
//	boost::shared_ptr<char> SharedBuffer;
	bool deleteMemWhenDrop;
	long Len;
	long Pos;
	std::string Filename;

};

#endif // __C_MEMORY_READ_FILE_H_INCLUDED__

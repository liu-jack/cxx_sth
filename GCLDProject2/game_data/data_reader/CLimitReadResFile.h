// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_LIMIT_READ_RES_FILE_H_INCLUDED__
#define __C_LIMIT_READ_RES_FILE_H_INCLUDED__

#pragma once

#include "IReadResFile.h"
#include <string>


/*! this is a read file, which is limited to some boundaries, 
 * so that it may only start from a certain file position
 * and may only read until a certain file position.
 * This can be useful, for example for reading uncompressed files
 * in an archive (zip).
!*/
class CLimitReadResFile : public IReadResFile
{
public:

	CLimitReadResFile(IReadResFile* alreadyOpenedFile, long areaSize, const char* name);

	virtual ~CLimitReadResFile();

	virtual IReadResFile* clone() const;

	//! returns how much was read
	virtual S32 read(void* buffer, U32 sizeToRead);

	//! changes position in file, returns true if successful
	//! if relativeMovement==true, the pos is changed relative to current pos,
	//! otherwise from begin of file
	virtual bool seek(long finalPos, bool relativeMovement = false);

	//! returns size of file
	virtual long getSize() const;

	//! returns where in the file we are.
	virtual long getPos() const;

	//! returns name of file
	virtual const char* getFileName() const;

	//! return complete path
	virtual const char* getFullPath() const;
private:

	void init();

	std::string Filename;
	long AreaSize;
	long AreaStart;
	long AreaEnd;
	IReadResFile* File;

	long LastPos;
};

#endif // __C_LIMIT_READ_RES_FILE_H_INCLUDED__

#ifndef __C_LIMIT_READ_RES_FILE_H_INCLUDED__
#define __C_LIMIT_READ_RES_FILE_H_INCLUDED__

#pragma once

#include "def/TypeDef.h"
#include "IReadResFile.h"
#include <string>

/*!
Class for reading a real file from disk.
*/
class CReadResFile : public IReadResFile
{
public:
	CReadResFile(const char* fileName);

	virtual IReadResFile* clone() const;

	virtual ~CReadResFile();

	//! returns how much was read
	virtual S32 read(void* buffer, U32 sizeToRead);

	//! changes position in file, returns true if successful
	virtual bool seek(long finalPos, bool relativeMovement = false);

	//! returns size of file
	virtual long getSize() const;

	//! returns if file is open
	virtual bool isOpen() const
	{
#if defined(GLITCH_WII_PLATFORM) && !defined(GLITCH_WII_VIEWER)
		return IsOpen;
#else
		return File != 0;
#endif
	}

	//! returns where in the file we are.
	virtual long getPos() const;

	//! returns name of file
	virtual const char* getFileName() const;

	//! returns complete path
	virtual const char* getFullPath() const;

private:

	//! opens the file
	void openFile();


	FILE* File;


	long FileSize;
	std::string Filename;
};

#endif // __C_LIMIT_READ_RES_FILE_H_INCLUDED__

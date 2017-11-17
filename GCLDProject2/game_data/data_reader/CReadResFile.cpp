//#include "preHeader.h"
//#include "stdafx.h"
#include "CReadResFile.h"

#ifdef WIN32
#	include <direct.h>
#endif

CReadResFile::CReadResFile(const char* fileName)
	: File(0)
	, FileSize(0)
	, Filename(fileName)
{
	#ifdef _DEBUG
	setDebugName("CReadResFile");
	#endif

	openFile();
}


CReadResFile::~CReadResFile()
{
	if (File)
		fclose(File);
}

IReadResFile* CReadResFile::clone() const
{
	CReadResFile* rf = new CReadResFile(this->getFullPath());
	rf->seek(this->getPos());
	return rf;
}

//! returns how much was read
int32 CReadResFile::read(void* buffer, uint32 sizeToRead)
{
	if (!isOpen())
		return 0;

#if defined(_IRR_WII_PLATFORM_) && !defined(_IRR_WII_VIEWER_)
    int32 bytesRead = (FileSize - Offset) < sizeToRead ? (FileSize - Offset) : sizeToRead;
    memcpy(buffer, Buffer + Padding + Offset, bytesRead);
    Offset += bytesRead;
    return bytesRead;
#else
	return (int32)fread(buffer, 1, sizeToRead, File);
#endif
}


//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CReadResFile::seek(long finalPos, bool relativeMovement)
{
	if (!isOpen())
		return false;
        
#if defined(_IRR_WII_PLATFORM_) && !defined(_IRR_WII_VIEWER_)
    int32 ofs = relativeMovement ? finalPos : (finalPos - Offset);
    if (Offset + ofs < FileSize)
    {
        Offset += ofs;
        return true;
    }
    return false;
#else
	return fseek(File, finalPos, relativeMovement ? SEEK_CUR : SEEK_SET) == 0;
#endif
}


//! returns size of file
long CReadResFile::getSize() const
{
	return FileSize;
}


//! returns where in the file we are.
long CReadResFile::getPos() const
{
#if defined(_IRR_WII_PLATFORM_) && !defined(_IRR_WII_VIEWER_)
    return Offset;
#else
	return ftell(File);
#endif
}


//! opens the file
void CReadResFile::openFile()
{
	if (Filename.size() == 0) // bugfix posted by rt
	{
		return;
	}
//#ifdef WIN32
//	fopen_s(&File, Filename.c_str(), "rb");
//#else
	File = fopen(Filename.c_str(), "rb");
//#endif
	//File = io::CFileSystem::open(Filename.c_str(), "rb");	

	if (File)
	{
		// get FileSize
		fseek(File, 0, SEEK_END);
		FileSize = getPos();
        fseek(File, 0, SEEK_SET);
	}
	else
	{
#ifdef WIN32
#ifdef _MMO_GAME_
		char path[256];
		getcwd(path, 256);
		//printf("Can not open file %s\\%s, errno = %d.\n", path, Filename.c_str(), errno);
#endif
#endif
	}
}


//! returns name of file
const char* CReadResFile::getFileName() const
{
	return Filename.c_str();
}

//! return full path
const char* CReadResFile::getFullPath() const
{
	return Filename.c_str();
}


IReadResFile* createReadFile(const char* fileName)
{
	CReadResFile* file = new CReadResFile(fileName);
	if (file->isOpen())
		return file;

	file->drop();
	return 0;
}


//#include "preHeader.h"
// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//#include "stdafx.h"
#include "CZipResReader.h"
#include <zlib.h>

CZipResReader::CZipResReader(const char* filename, bool ignoreCase, bool ignorePaths)
	: IgnoreCase(ignoreCase)
	, IgnorePaths(ignorePaths)
{
#ifdef _DEBUG
	setDebugName("CZipResReader");
#endif

	File = createReadFile(filename);

	//GLITCH_DEBUG_BREAK_if (!isValid(File));

	if (File)
	{
		// scan local headers
		while (scanLocalHeader());

		// prepare file index for binary search
		if (FileList.size() >= 2)
		{
			std::sort(FileList.begin(), FileList.end());
			//core::heapsort(&FileList[0], FileList.size());
		}
	}
}

CZipResReader::CZipResReader(IReadResFile* file, bool ignoreCase, bool ignorePaths)
	: File(file)
	, IgnoreCase(ignoreCase)
	, IgnorePaths(ignorePaths)
{
	//#ifdef _DEBUG
	//setDebugName("CZipReader");
	//#endif

	if (File)
	{
		File->grab();

		// scan local headers
		while (scanLocalHeader());

		// prepare file index for binary search
		if (FileList.size() >= 2)
		{
			std::sort(FileList.begin(), FileList.end());
			//core::heapsort(&FileList[0], FileList.size());
		}
	}
}

CZipResReader::~CZipResReader()
{
	if (File)
		File->drop();
}

//! splits filename from zip file into useful filenames and paths
void
CZipResReader::extractFilename(SZipResFileEntry* entry)
{
	S32 lorfn = entry->header.FilenameLength; // length of real file name

	if (!lorfn)
		return;

	if (IgnoreCase)
	{
		for (U32 i=0; i<entry->zipFileName.length(); ++i)
		{
			char x = entry->zipFileName[i];
			entry->zipFileName[i] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
		}
	}
	const char* p = entry->zipFileName.c_str() + lorfn;
	
	// suche ein slash oder den anfang.

	while (*p!='/' && p!=entry->zipFileName.c_str())
	{
		--p;
		--lorfn;
	}

	bool thereIsAPath = p != entry->zipFileName.c_str();

	if (thereIsAPath)
	{
		// there is a path
		++p;
		++lorfn;
	}

	entry->simpleFileName = p;
	entry->path = "";

	// pfad auch kopieren
	if (thereIsAPath)
	{
		lorfn = (S32)(p - entry->zipFileName.c_str());
		
		entry->path = entry->zipFileName.substr( 0, lorfn );

		//entry->path.append(entry->zipFileName, lorfn);
		//entry->path.append ( "" );
	}

	if (!IgnorePaths)
		entry->simpleFileName = entry->zipFileName; // thanks to Pr3t3nd3r for this fix
}

//! scans for a local header, returns false if there is no more local file header.
bool
CZipResReader::scanLocalHeader()
{
	char tmp[1024];

	SZipResFileEntry entry;
	entry.fileDataPosition = 0;
	memset(&entry.header, 0, sizeof(SZIPResFileHeader));

	File->read(&entry.header, sizeof(SZIPResFileHeader));

#ifdef __BIG_ENDIAN__
	entry.header.Sig = os::byteswap(entry.header.Sig);
	entry.header.VersionToExtract = os::byteswap(entry.header.VersionToExtract);
	entry.header.GeneralBitFlag = os::byteswap(entry.header.GeneralBitFlag);
	entry.header.CompressionMethod = os::byteswap(entry.header.CompressionMethod);
	entry.header.LastModFileTime = os::byteswap(entry.header.LastModFileTime);
	entry.header.LastModFileDate = os::byteswap(entry.header.LastModFileDate);
	entry.header.DataDescriptor.CRC32 = os::byteswap(entry.header.DataDescriptor.CRC32);
	entry.header.DataDescriptor.CompressedSize = os::byteswap(entry.header.DataDescriptor.CompressedSize);
	entry.header.DataDescriptor.UncompressedSize = os::byteswap(entry.header.DataDescriptor.UncompressedSize);
	entry.header.FilenameLength = os::byteswap(entry.header.FilenameLength);
	entry.header.ExtraFieldLength = os::byteswap(entry.header.ExtraFieldLength);
#endif

	if (entry.header.Sig != 0x04034b50 && entry.header.Sig != 0x504d4247)
		return false; // local file headers end here.

	// read filename
	entry.zipFileName.reserve(entry.header.FilenameLength+2);
	File->read(tmp, entry.header.FilenameLength);
	tmp[entry.header.FilenameLength] = 0x0;
	entry.zipFileName = tmp;

	extractFilename(&entry);

	// move forward length of extra field.

	if (entry.header.ExtraFieldLength)
		File->seek(entry.header.ExtraFieldLength, true);

	// if bit 3 was set, read DataDescriptor, following after the compressed data
	if (entry.header.GeneralBitFlag & ZIP_RES_INFO_IN_DATA_DESCRITOR)
	{
		// read data descriptor
		File->read(&entry.header.DataDescriptor, sizeof(entry.header.DataDescriptor));
#ifdef __BIG_ENDIAN__
		entry.header.DataDescriptor.CRC32 = os::byteswap(entry.header.DataDescriptor.CRC32);
		entry.header.DataDescriptor.CompressedSize = os::byteswap(entry.header.DataDescriptor.CompressedSize);
		entry.header.DataDescriptor.UncompressedSize = os::byteswap(entry.header.DataDescriptor.UncompressedSize);
#endif
	}

	// store position in file
	entry.fileDataPosition = File->getPos();

	// move forward length of data
	File->seek(entry.header.DataDescriptor.CompressedSize, true);

	#ifdef _DEBUG
	//os::Debuginfo::print("added file from archive", entry.simpleFileName.c_str());
	#endif

	FileList.push_back(entry);

	return true;
}

//! opens a file by file name
IReadResFile*
CZipResReader::openFile(const char* filename)
{
	S32 index = findFile(filename);

	if (index != -1)
		return openFile(index);

	return 0;
}

//! opens a file by index
IReadResFile*
CZipResReader::openFile(S32 index)
{
	//0 - The file is stored (no compression)
	//1 - The file is Shrunk
	//2 - The file is Reduced with compression factor 1
	//3 - The file is Reduced with compression factor 2
	//4 - The file is Reduced with compression factor 3
	//5 - The file is Reduced with compression factor 4
	//6 - The file is Imploded
	//7 - Reserved for Tokenizing compression algorithm
	//8 - The file is Deflated
	//9 - Reserved for enhanced Deflating
	//10 - PKWARE Date Compression Library Imploding

	switch(FileList[index].header.CompressionMethod)
	{
	case 0: // no compression
		{
#if 0
			IReadResFile * file = NULL;

			{
				boost::mutex::scoped_lock l( mutex ) ;

				//pthread_mutex_lock(&mutex);
				File->seek(FileList[index].fileDataPosition);
				file = createLimitReadFile(FileList[index].simpleFileName.c_str(), File, FileList[index].header.DataDescriptor.UncompressedSize);
				//pthread_mutex_unlock(&mutex);
			}

			return file;
#else
			const U32 uncompressedSize = FileList[index].header.DataDescriptor.UncompressedSize;
			if (uncompressedSize > 0)
			{
				void* pBuf = new char[ uncompressedSize ];
				if (!pBuf)
				{
					printf("Not enough memory for read file %s", FileList[index].simpleFileName.c_str());
					return 0;
				}
				
				{
					boost::mutex::scoped_lock l( mutex ) ;
					//pthread_mutex_lock(&mutex);
					File->seek(FileList[index].fileDataPosition);
					File->read(pBuf, uncompressedSize);
					//pthread_mutex_unlock(&mutex);
				}

				return createMemoryReadFile(pBuf, uncompressedSize, FileList[index].zipFileName.c_str(), true);
			}
			return 0;
#endif
		}
	case 8:
		{
 			
			const U32 uncompressedSize = FileList[index].header.DataDescriptor.UncompressedSize;			
			const U32 compressedSize = FileList[index].header.DataDescriptor.CompressedSize;

			void* pBuf = new char[ uncompressedSize ];
			if (!pBuf)
			{
				printf("Not enough memory for decompressing %s", FileList[index].simpleFileName.c_str());
				return 0;
			}

			char *pcData = new char[ compressedSize ];
			if (!pcData)
			{
				printf("Not enough memory for decompressing %s", FileList[index].simpleFileName.c_str());
				return 0;
			}

			{
				boost::mutex::scoped_lock l( mutex ) ;

				//pthread_mutex_lock(&mutex);
				//memset(pcData, 0, compressedSize );
				File->seek(FileList[index].fileDataPosition);
				File->read(pcData, compressedSize );
				//pthread_mutex_unlock(&mutex);
			}

			// Setup the inflate stream.
			z_stream stream;
			S32 err;

			stream.next_in = (Bytef*)pcData;
			stream.avail_in = (uInt)compressedSize;
			stream.next_out = (Bytef*)pBuf;
			stream.avail_out = uncompressedSize;
			stream.zalloc = (alloc_func)0;
			stream.zfree = (free_func)0;

			// Perform inflation. wbits < 0 indicates no zlib header inside the data.
			err = inflateInit2(&stream, -MAX_WBITS);
			if (err == Z_OK)
			{
				err = inflate(&stream, Z_FINISH);
				inflateEnd(&stream);
				if (err == Z_STREAM_END)
					err = Z_OK;
				err = Z_OK;
				inflateEnd(&stream);
			}


			delete[] pcData;
			
			if (err != Z_OK)
			{
		//		DLOG("Error decompressing %s", FileList[index].simpleFileName.c_str());
				delete [] (char*)pBuf;
				return 0;
			}
			else
				return createMemoryReadFile(pBuf, uncompressedSize, FileList[index].zipFileName.c_str(), true);
			

		}
	default:
		//DLOG("file %s has unsupported compression method.", FileList[index].simpleFileName.c_str());
		return 0;
	};
}

//! returns count of files in archive
S32
CZipResReader::getFileCount()
{
	return FileList.size();
}

//! returns data of file
const SZipResFileEntry*
CZipResReader::getFileInfo(S32 index) const
{
	return &FileList[index];
}

//! deletes the path from a filename
void
CZipResReader::deletePathFromFilename(std::string& filename)
{
	// delete path from filename
	const char* p = filename.c_str() + filename.size();

	// search for path separator or beginning

	while (*p!='/' && *p!='\\' && p!=filename.c_str())
		--p;

	if (p != filename.c_str())
	{
		++p;
		filename = p;
	}
}

//! returns fileindex
S32
CZipResReader::findFile(const char* simpleFilename)
{
	SZipResFileEntry entry;
	if (simpleFilename[0] == '.' && simpleFilename[1] == '/')
		entry.simpleFileName = simpleFilename + 2;
	else
		entry.simpleFileName = simpleFilename;

	if (IgnoreCase)
	{
		for (U32 i=0; i<entry.simpleFileName.length(); ++i)
		{
			char x = entry.simpleFileName[i];
			entry.simpleFileName[i] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
		}
	}
	if (IgnorePaths)
		deletePathFromFilename(entry.simpleFileName);

	S32 res = my_binary_search(FileList, entry);

	//#ifdef _DEBUG
	//if (res == -1)
	//{
	//	for (U32 i=0; i<FileList.size(); ++i)
	//		if (FileList[i].simpleFileName == entry.simpleFileName)
	//		{
	//			DLOG("File %s in archive but not found.", entry.simpleFileName.c_str());
	//			break;
	//		}
	//}
	//#endif

	return res;
}

bool 
CZipResReader::isValid(IReadResFile* file)
{
	const char zipTag[] = {'P', 'K', 0x03, 0x04};
	char header[4];
	int pos = file->getPos();
	file->read(header, 4);
	file->seek(pos);
	return (*(int*)header == *(int*)zipTag);
}

bool 
CZipResReader::isValid(const char * filename)
{
	IReadResFile *readFile = createReadFile(filename);
	bool validation = isValid(readFile);
	readFile->drop();
	return validation;
}
#if 0
// -----------------------------------------------------------------------------
#if 1

class CUnzipReadFile 
#if defined(GLITCH_COMPILE_WITH_GLF)
	: public CGlfReadFile
#else
	: public IReadFile
#endif
{
public:

	CUnzipReadFile( const std::string &realName, const char * hashName )
#if defined(GLITCH_COMPILE_WITH_GLF)
		: CGlfReadFile(realName.c_str())
#else
		: IReadResFile(realName.c_str())
#endif
	{
		CallFileName = hashName;
	}

	virtual const char* getFileName() const
	{
		return CallFileName.c_str ();
	}

	std::string CallFileName;
};

CUnZipReader::CUnZipReader( IFileSystem * parent, const char* basename, bool ignoreCase, bool ignorePaths)
	: CZipResReader((IReadResFile *)0, ignoreCase, ignorePaths)
	, Parent(parent)
{
	Base = basename;
	if (Base [ Base.size() - 1 ] != '\\'
		&& Base [ Base.size() - 1 ] != '/')
	{
		Base += "/";
	}
}

void
CUnZipReader::buildDirectory()
{
}

//! opens a file by file name
IReadResFile*
CUnZipReader::openFile(const char* filename)
{
	std::string fname;
	fname = Base;
	fname += filename;

	CUnzipReadFile* file = new CUnzipReadFile( fname, filename);
	if (file->isOpen())
		return file;

	file->drop();
	return 0;

}

//! returns fileindex
S32
CUnZipReader::findFile(const char* filename)
{
	IReadResFile *file = openFile ( filename );
	if ( 0 == file )
		return -1;
	file->drop ();
	return 1;
}

#else

CUnZipReader::CUnZipReader(IFileSystem * parent,
						   const char* basename,
						   bool ignoreCase,
						   bool ignorePaths)
	: CZipResReader(0, ignoreCase, ignorePaths)
	, Parent(parent)
{
	strcpy ( Buf, Parent->getWorkingDirectory () );

	Parent->changeWorkingDirectoryTo ( basename );
	buildDirectory ( );
	Parent->changeWorkingDirectoryTo ( Buf );

	if (FileList.size() >= 2)
	{
		core::heapsort(&FileList[0], FileList.size());
	}
}

void
CUnZipReader::buildDirectory()
{
	IFileList * list = new CFileList();

	SZipFileEntry entry;

	const U32 size = list->getFileCount();
	for (U32 i = 0; i!= size; ++i)
	{
		if (false == list->isDirectory(i))
		{
			entry.zipFileName = list->getFullFileName ( i );
			entry.header.FilenameLength = entry.zipFileName.size ();
			extractFilename(&entry);
			FileList.push_back(entry);
		}
		else
		{
			const char * rel = list->getFileName ( i );

			if (strcmp( rel, "." ) && strcmp( rel, ".." ))
			{
				Parent->changeWorkingDirectoryTo ( rel );
				buildDirectory ();
				Parent->changeWorkingDirectoryTo ( ".." );
			}
		}
	}

	list->drop ();
}

//! opens a file by file name
IReadResFile*
CUnZipReader::openFile(const char* filename)
{
	S32 index = -1;

	if ( IgnorePaths )
	{
		index = findFile(filename);
	}
	else
	if ( FileList.size () )
	{
		const std::string search = FileList[0].path + filename;
		index = findFile( search.c_str() );
	}

	if (index == -1)
		return 0;

	return createReadFile(FileList[index].zipFileName.c_str() );
}
#endif
#endif
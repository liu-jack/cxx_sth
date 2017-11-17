//#include "preHeader.h"
// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

//#include "stdafx.h"
#include "CReadResFile.h"
#include "CPackResReader.h"

#include <zlib.h>
#include "lzma/LzmaDec.h"

#define MAX_FILE_NAME_LEN 1024
namespace
{
	void *SzAlloc(void *p, size_t size) { p = p; return malloc(size); }
	void SzFree(void *p, void *address) { p = p; free(address); }
	ISzAlloc lzmaAlloc = { SzAlloc, SzFree };
}
unsigned int hashStringSimple(const char* ptr)
{
	unsigned int hash = 0;
	for (; *ptr; ++ptr)
	{
		hash = (hash * 13) + (*ptr);
	}
	return hash;
}

CPackResReader::CPackResReader(const char* filename, bool ignoreCase, bool ignorePaths)
	: m_fileNb(0)
	, m_fileNameBuffer(NULL)
//	, IgnoreCase(ignoreCase)
//	, IgnorePaths(ignorePaths)
{
#ifdef _DEBUG
	setDebugName("CPackResReader");
#endif
	
	//pthread_mutex_init(&mutex, NULL);

	m_file = createReadFile(filename);

	//GLITCH_DEBUG_BREAK_if (!isValid(m_file));

	if (m_file)
	{
		// scan local headers
		scanFileHeader();

		// prepare file index for binary search
		if (m_fileList.size() >= 2)
		{
			std::sort(m_fileList.begin(), m_fileList.end());
			//core::heapsort(&m_fileList[0], m_fileList.size());
#ifdef _DEBUG
			{
				int sumConflict = 0;
				for (int i = 0; i< m_fileNb - 1; i++)
				{
					if (m_fileList[i].filePathHash == m_fileList[i+1].filePathHash)
						sumConflict++;
				}
				if (sumConflict > 0)
					printf("Hash Conflict %d\n", sumConflict);
			}
#endif
		}
	}
}

CPackResReader::CPackResReader(IReadResFile* file, bool ignoreCase, bool ignorePaths)
	: m_file(file)
	, m_fileNb(0)
	, m_fileNameBuffer(NULL)
//	, IgnoreCase(ignoreCase)
//	, IgnorePaths(ignorePaths)
{
//#ifdef _DEBUG
//	setDebugName("CPackReader");
//#endif
	//pthread_mutex_init(&mutex, NULL);

	if (m_file)
	{
		m_file->grab();

		//// scan local headers
		scanFileHeader();

		//// prepare file index for binary search
		if (m_fileList.size() >= 2)
		{
			std::sort(m_fileList.begin(), m_fileList.end());
			//core::heapsort(&m_fileList[0], m_fileList.size());
#ifdef _DEBUG
			{
				int sumConflict = 0;
				for (int i = 0; i < m_fileNb - 1; i++)
				{
					if (m_fileList[i].filePathHash == m_fileList[i+1].filePathHash)
						sumConflict++;
				}
				if (sumConflict > 0)
					printf("Hash Conflict %d\n", sumConflict);
			}
#endif
		}
	}
}

CPackResReader::~CPackResReader()
{
	if (m_file)
		m_file->drop();
	if (m_fileNameBuffer)
	{
		delete []m_fileNameBuffer;
		m_fileNameBuffer = NULL;
	}
	//pthread_mutex_destroy(&mutex);
}

//! splits filename from Pack file into useful filenames and paths
//void
//CPackResReader::extractFilename(SPackResFileEntry* entry)
//{
//	S32 lorfn = entry->header.FilenameLength; // length of real file name
//
//	if (!lorfn)
//		return;
//
//	if (IgnoreCase)
//	{
//		for (U32 i = 0; i<entry->PackFileName.length(); ++i)
//		{
//			char x = entry->PackFileName[i];
//			entry->PackFileName[i] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
//		}
//	}
//	const char* p = entry->PackFileName.c_str() + lorfn;
//	
//	// suche ein slash oder den anfang.
//
//	while (*p!='/' && p!=entry->PackFileName.c_str())
//	{
//		--p;
//		--lorfn;
//	}
//
//	bool thereIsAPath = p != entry->PackFileName.c_str();
//
//	if (thereIsAPath)
//	{
//		// there is a path
//		++p;
//		++lorfn;
//	}
//
//	entry->simpleFileName = p;
//	entry->path = "";
//
//	// pfad auch kopieren
//	if (thereIsAPath)
//	{
//		lorfn = (S32)(p - entry->PackFileName.c_str());
//		
//		entry->path = entry->PackFileName.substr( 0, lorfn );
//
//		//entry->path.append(entry->PackFileName, lorfn);
//		//entry->path.append ( "" );
//	}
//
//	if (!IgnorePaths)
//		entry->simpleFileName = entry->PackFileName; // thanks to Pr3t3nd3r for this fix
//}

//! scans for a local header, returns false if there is no more local file header.
#pragma pack(push,1)

/*
F.  Central directory structure:

      [file header 1]
      .
      .
      . 
      [file header n]
      [digital signature] 


 central file header signature   4 bytes  (0x02014b50)
        version made by                 2 bytes
        version needed to extract       2 bytes
        general purpose bit flag        2 bytes
        compression method              2 bytes
        last mod file time              2 bytes
        last mod file date              2 bytes
        crc-32                          4 bytes
        compressed size                 4 bytes
        uncompressed size               4 bytes
        file name length                2 bytes
        extra field length              2 bytes
        file comment length             2 bytes
        disk number start               2 bytes
        internal file attributes        2 bytes
        external file attributes        4 bytes
        relative offset of local header 4 bytes

        file name (variable size)
        extra field (variable size)
        file comment (variable size)
*/

struct SZIPCenterDirFileHeader
{
	int32 Sig;							//	 4 bytes		(0x02014b50)
	int16 VersionMadeBy;				//	 2 bytes
	int16 VersionToExtract;				//	 2 bytes
	int16 GeneralBitFlag;				//	 2 bytes
	int16 CompressionMethod;			//	 2 bytes
	int16 LastModFileTime;				//	 2 bytes
	int16 LastModFileDate;				//	 2 bytes
	int32 CRC32;						//	 4 bytes
	int32 CompressedSize;				//	 4 bytes
	int32 UncompressedSize;				//	 4 bytes
	int16 FilenameLength;				//	 2 bytes
	int16 ExtraFieldLength;				//	 2 bytes
	int16 FileCommentLength;			//	 2 bytes
	int16 DiskNumberStart;				//   2 bytes
    int16 InternalFileAttributes;		//   2 bytes
    int32 ExternalFileAttributes;		//   4 bytes
    int32 RelativeOffsetOfLocalHeader;	//	 4 bytes

	//file name (variable size)			//  FilenameLength
	//extra field (variable size)		//  ExtraFieldLength
	//file comment (variable size)		//  FileCommentLength
} ;
/*
  I.  End of central directory record:

        end of central dir signature													4 bytes  (0x06054b50)
        number of this disk																2 bytes
        number of the disk with the start of the central directory						2 bytes
        total number of entries in the central directory on this disk					2 bytes
        total number of entries in the central directory								2 bytes
        size of the central directory													4 bytes
        offset of start of central directory with respect to the starting disk number	4 bytes
        .ZIP file comment length														2 bytes
        .ZIP file comment																(variable size)
*/
struct SZIPEndCenterDirRecord
{
	int32 Sig;							//	 4 bytes		(0x06054b50)
	int16 ThisDiskNo;					//	 2 bytes
	int16 CenterDirStartDiskNo;			//	 2 bytes
	int16 EntryTotalNumThisDisk;		//	 2 bytes
	int16 EntryTotalNum;				//	 2 bytes
	int32 CenterDirSize;				//	 4 bytes
	int32 CenterDirOffset;				//	 4 bytes
	int16 ZipFileCommentLength;			//	 2 bytes
	//ZIP file comment (variable size)		//  ZipFileCommentLength
} ;
// Default alignment
#pragma pack(pop)

void
CPackResReader::scanFileHeader()
{
	int fileSize = m_file->getSize();
	if ( static_cast<size_t>( fileSize ) < sizeof(SZIPEndCenterDirRecord)) //the pack file's length is too short
		return;

	m_file->seek(fileSize - sizeof(SZIPEndCenterDirRecord));
	SZIPEndCenterDirRecord endCenterDirRecord;
	m_file->read(&endCenterDirRecord, sizeof(SZIPEndCenterDirRecord));
	
	if (endCenterDirRecord.Sig == 0x06054b50 )  //"PK 05 06"
	{
		m_fileNb = endCenterDirRecord.EntryTotalNum;
		m_fileList.reserve(m_fileNb);
		
		m_file->seek(endCenterDirRecord.CenterDirOffset);
		char *buffer = new char[endCenterDirRecord.CenterDirSize];
		m_file->read(buffer, endCenterDirRecord.CenterDirSize);
		
		SZIPCenterDirFileHeader *pHead =(SZIPCenterDirFileHeader *)buffer;
		char zipFileName[MAX_FILE_NAME_LEN]; 
		int i = 0;
		int fileNameBuffLength = 0;
		SPackResFileEntry entry;
		SZIPResFileHeader zipFileHeader;
		for (; i< m_fileNb; i++)
		{
			if (pHead->Sig != 0x02014b50) //Error
				break;
			//Calculate Hash
			char * pFileName = (char *)(pHead + 1);
			for (int j=0; j<pHead->FilenameLength; j++)
			{
				char x = pFileName[j];
				zipFileName[j] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
			}
			zipFileName[pHead->FilenameLength] = 0;

			entry.filePathHash = hashStringSimple(zipFileName);
			entry.fileDataPosition = pHead->RelativeOffsetOfLocalHeader;
			entry.fileName = NULL;
			fileNameBuffLength += (pHead->FilenameLength + 4) & ~3;
			//Read Other Information from Local Header, slow but can accelerate dynamic loading in Action Phase
			{
				m_file->seek(entry.fileDataPosition);
				m_file->read(&zipFileHeader, sizeof(SZIPResFileHeader));

				if (zipFileHeader.Sig != 0x04034b50 && zipFileHeader.Sig != 0x504d4247)
				{
					break; // local file headers end here.
				}

				entry.localheaderSize = sizeof(SZIPResFileHeader) + zipFileHeader.FilenameLength  + zipFileHeader.ExtraFieldLength;
				// if bit 3 was set, read DataDescriptor, following after the compressed data
				if (zipFileHeader.GeneralBitFlag & ZIP_RES_INFO_IN_DATA_DESCRITOR)
				{
					m_file->seek(zipFileHeader.FilenameLength + zipFileHeader.ExtraFieldLength, true);
					// read data descriptor
					m_file->read(&zipFileHeader.DataDescriptor, sizeof(zipFileHeader.DataDescriptor));
					entry.localheaderSize += sizeof(zipFileHeader.DataDescriptor);

				}
				entry.compressionMethod = zipFileHeader.CompressionMethod;
				entry.uncompressedSize = zipFileHeader.DataDescriptor.UncompressedSize;
				entry.compressedSize = zipFileHeader.DataDescriptor.CompressedSize;
				entry.generalBitFlag = zipFileHeader.GeneralBitFlag;
			}

			m_fileList.push_back(entry);

			pHead = (SZIPCenterDirFileHeader *)(pFileName + pHead->FilenameLength + pHead->ExtraFieldLength + pHead->FileCommentLength);
		}
		if (i == m_fileNb) //if success, return ,else scan for start
		{
			m_fileNameBuffer = new char[fileNameBuffLength];
			memset(m_fileNameBuffer, 0, fileNameBuffLength);
			fileNameBuffLength = 0;
			pHead =(SZIPCenterDirFileHeader *)buffer;
			for (i = 0; i< m_fileNb; i++)
			{
				if (pHead->Sig != 0x02014b50) //Error
					break;
				//Calculate Hash
				char * pFileName = (char *)(pHead + 1);
				for (int j=0; j<pHead->FilenameLength; j++)
				{
					char x = pFileName[j];
					m_fileNameBuffer[fileNameBuffLength + j] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
				}

				m_fileList[i].fileName = m_fileNameBuffer + fileNameBuffLength;
				fileNameBuffLength += (pHead->FilenameLength + 4) & ~3;
				pHead = (SZIPCenterDirFileHeader *)(pFileName + pHead->FilenameLength + pHead->ExtraFieldLength + pHead->FileCommentLength);
			}
			
			delete[] buffer;

			return;
		}
		delete[] buffer;

	}
	
	//for compatible, not found end CenterDir Record, scan from start
	{
		m_file->seek(0);
		m_fileNb = 0;
		char zipFileName[MAX_FILE_NAME_LEN]; 
		SZIPResFileHeader zipFileHeader;
		int fileNameBuffLength = 0;
		std::vector<std::string> fileNameStringVector;
		while(true)
		{
			SPackResFileEntry entry;
			entry.fileDataPosition = m_file->getPos();
			
			memset(&zipFileHeader, 0, sizeof(SZIPResFileHeader));			
			m_file->read(&zipFileHeader, sizeof(SZIPResFileHeader));

			if (zipFileHeader.Sig != 0x04034b50 && zipFileHeader.Sig != 0x504d4247)
				break; // local file headers end here.

			// read filename
			m_file->read(zipFileName, zipFileHeader.FilenameLength);
			zipFileName[zipFileHeader.FilenameLength] = 0;
			fileNameStringVector.push_back(zipFileName);

			// move forward length of extra field.
			if (zipFileHeader.ExtraFieldLength)
				m_file->seek(zipFileHeader.ExtraFieldLength, true);
			
			// if bit 3 was set, read DataDescriptor, following after the compressed data
			if (zipFileHeader.GeneralBitFlag & ZIP_RES_INFO_IN_DATA_DESCRITOR)
			{
				// read data descriptor
				m_file->read(&zipFileHeader.DataDescriptor, sizeof(zipFileHeader.DataDescriptor));
				
			}

			entry.generalBitFlag = zipFileHeader.GeneralBitFlag;


			m_file->seek(zipFileHeader.DataDescriptor.CompressedSize, true);
			
			
			for (int i = 0; i<zipFileHeader.FilenameLength; i++)
			{
				char x = zipFileName[i];
				zipFileName[i] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
			}
			entry.filePathHash = hashStringSimple(zipFileName);
			entry.fileName = 0;
			fileNameBuffLength += (zipFileHeader.FilenameLength + 4) & ~3;

			m_fileList.push_back(entry);
			m_fileNb++;

		}
		m_fileNameBuffer = new char[fileNameBuffLength];
		memset(m_fileNameBuffer, 0, fileNameBuffLength);
		fileNameBuffLength = 0;
		for (int i = 0; i< m_fileNb; i++)
		{
			memcpy(m_fileNameBuffer + fileNameBuffLength, fileNameStringVector[i].c_str(), fileNameStringVector[i].length());
			m_fileList[i].fileName = m_fileNameBuffer + fileNameBuffLength;
			fileNameBuffLength += (fileNameStringVector[i].length() + 4) &~3;

		}

		

	}
	//return true;
}

//! opens a file by file name
IReadResFile*
CPackResReader::openFile(const char* filename)
{
	S32 index = findFile(filename);
	if (index != -1)
		return openFile(index);

	return 0;
}

//! opens a file by index
IReadResFile*
CPackResReader::openFile(S32 index)
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

	SPackResFileEntry& fileEntry = m_fileList[index];

	if (fileEntry.localheaderSize == 0)
	{

		SZIPResFileHeader zipFileHeader;
		U32 zipFileHeaderLength; 


		{
			AutoMutexLocker l( mutex ) ;

			//pthread_mutex_lock(&mutex); //Must lock for thread safe
			m_file->seek(fileEntry.fileDataPosition);
			m_file->read(&zipFileHeader, sizeof(SZIPResFileHeader));

			if (zipFileHeader.Sig != 0x04034b50 && zipFileHeader.Sig != 0x504d4247)
			{
				//pthread_mutex_unlock(&mutex);
				return NULL; // local file headers end here.
			}

			zipFileHeaderLength = sizeof(SZIPResFileHeader) + zipFileHeader.FilenameLength  + zipFileHeader.ExtraFieldLength;
			// if bit 3 was set, read DataDescriptor, following after the compressed data
			if (zipFileHeader.GeneralBitFlag & ZIP_RES_INFO_IN_DATA_DESCRITOR)
			{
				m_file->seek(zipFileHeader.FilenameLength + zipFileHeader.ExtraFieldLength, true);
				// read data descriptor
				m_file->read(&zipFileHeader.DataDescriptor, sizeof(zipFileHeader.DataDescriptor));
				zipFileHeaderLength += sizeof(zipFileHeader.DataDescriptor);

			}
			//pthread_mutex_unlock(&mutex);
		}

		fileEntry.localheaderSize = zipFileHeaderLength;
		fileEntry.compressionMethod = zipFileHeader.CompressionMethod;
		fileEntry.uncompressedSize = zipFileHeader.DataDescriptor.UncompressedSize;
		fileEntry.compressedSize = zipFileHeader.DataDescriptor.CompressedSize;
		fileEntry.generalBitFlag = zipFileHeader.GeneralBitFlag;
	}

	

	int realDataPos = fileEntry.fileDataPosition + fileEntry.localheaderSize;

	
	switch(fileEntry.compressionMethod)
	{
	case 0: // no compression
		{
#ifdef IPHONEOS
			IReadResFile * file = NULL;

			{
				AutoMutexLocker l( mutex ) ;
				m_file->seek(realDataPos);
				file = createLimitReadFile(fileEntry.fileName, m_file, fileEntry.uncompressedSize);
			}

			return file;
#else
			const U32 uncompressedSize = fileEntry.uncompressedSize;
			if (uncompressedSize > 0)
			{
				void* pBuf = new char[ uncompressedSize ];
				if (!pBuf)
				{
					printf("Not enough memory for read file %s",fileEntry.fileName);
					return 0;
				}

				{
					AutoMutexLocker l( mutex ) ;
					//pthread_mutex_lock(&mutex);
					m_file->seek(realDataPos);
					m_file->read(pBuf, uncompressedSize);
					//pthread_mutex_unlock(&mutex);
				}

				return createMemoryReadFile(pBuf, uncompressedSize, fileEntry.fileName, true);
			}
			return 0;
#endif
		}
	case 8:
		{
 			
			const U32 uncompressedSize = fileEntry.uncompressedSize;			
			const U32 compressedSize = fileEntry.compressedSize;

			void* pBuf = new char[ uncompressedSize ];
			if (!pBuf)
			{
				printf("Not enough memory for decompressing %s", fileEntry.fileName);
				return 0;
			}

			char *pcData = new char[ compressedSize ];
			if (!pcData)
			{
				printf("Not enough memory for decompressing %s", fileEntry.fileName);
				delete [] (char*)pBuf;
				return 0;
			}

			memset(pcData, 0, compressedSize );
			
			{
				AutoMutexLocker l( mutex ) ;
				//pthread_mutex_lock(&mutex);
				m_file->seek(realDataPos);
				m_file->read(pcData, compressedSize );
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
				//DLOG("Error decompressing %s", filename);
				delete [] (char*)pBuf;
				return 0;
			}
			else
				return createMemoryReadFile(pBuf, uncompressedSize, fileEntry.fileName, true);
			

		}
		case 14:
		{
  			//#ifdef _IRR_COMPILE_WITH_LZMA_

			U32 uncompressedSize = fileEntry.uncompressedSize;
			U8* pBuf = new U8[ uncompressedSize ];
			if (!pBuf)
			{
				//swprintf ( buf, 64, L"Not enough memory for decompressing %s", Files[index].FullName.c_str() );
				//os::Printer::log( buf, ELL_ERROR);
				//if (decrypted)
					//decrypted->drop();
				printf("Not enough memory for decompressing %s", fileEntry.fileName);
				return 0;
			}
			U32 decryptedSize=fileEntry.compressedSize;

			//U8 *pcData = decryptedBuf;
			U8 *pcData = NULL;
			//if (!pcData)
			//{
				pcData = new U8[decryptedSize];
				if (!pcData)
				{
					//swprintf ( buf, 64, L"Not enough memory for decompressing %s", Files[index].FullName.c_str() );
					//os::Printer::log( buf, ELL_ERROR);
					printf("Not enough memory for decompressing %s", fileEntry.fileName);
					delete [] pBuf;
					return 0;
				}

				//memset(pcData, 0, decryptedSize);

				{
					AutoMutexLocker l( mutex ) ;
					//pthread_mutex_lock(&mutex);
					m_file->seek(realDataPos);
					m_file->read(pcData, decryptedSize);
					//pthread_mutex_unlock(&mutex);
				}
			//}

			ELzmaStatus status;
			U32 tmpDstSize = uncompressedSize;
			U32 tmpSrcSize = decryptedSize;

			unsigned int propSize = (pcData[3]<<8)+pcData[2];
			int err = LzmaDecode((Byte*)pBuf, &tmpDstSize,
					pcData+4+propSize, &tmpSrcSize,
					pcData+4, propSize,
					(fileEntry.generalBitFlag&0x1)?LZMA_FINISH_END:LZMA_FINISH_ANY, &status,
					&lzmaAlloc);
			uncompressedSize = tmpDstSize; // may be different to expected value

			//if (decrypted)
			//	decrypted->drop();
			//else
				delete[] pcData;

			if (err != SZ_OK)
			{
				//os::Printer::log( "Error decompressing", Files[index].FullName, ELL_ERROR);
				printf("Error decompressing %s", fileEntry.fileName);
				delete [] pBuf;
				return 0;
			}
			else
				return createMemoryReadFile(pBuf, uncompressedSize, fileEntry.fileName, true);

			//#else
			//os::Printer::log("lzma decompression not supported. File cannot be read.", ELL_ERROR);
			//return 0;
			//#endif
		}
	default:
		//DLOG("file %s has unsupported compression method.", filename);
		break; // return 0;
	};
return 0;
}

//! returns count of files in archive
S32
CPackResReader::getFileCount()
{
	return m_fileList.size();
}

//! returns data of file
const SPackResFileEntry*
CPackResReader::getFileInfo(S32 index) const
{
	
//	const SPackResFileEntry &foundEntry = m_fileList[index];
	return &m_fileList[index];
}

//! deletes the path from a filename
//void
//CPackResReader::deletePathFromFilename(std::string& filename)
//{
//	// delete path from filename
//	const char* p = filename.c_str() + filename.size();
//
//	// search for path separator or beginning
//
//	while (*p!='/' && *p!='\\' && p!=filename.c_str())
//		--p;
//
//	if (p != filename.c_str())
//	{
//		++p;
//		filename = p;
//	}
//}

//! returns fileindex
S32
CPackResReader::findFile(const char* simpleFilename)
{
	std::string entryFileName;
	if (simpleFilename[0] == '.' && simpleFilename[1] == '/')
		entryFileName = simpleFilename + 2;
	else
		entryFileName = simpleFilename;
	SPackResFileEntry entry;
	//Calculate the Hash
	//if (IgnoreCase)
	{
		for (U32 i = 0; i<entryFileName.length(); ++i)
		{
			char x = entryFileName[i];
			entryFileName[i] = x >= 'A' && x <= 'Z' ? (char) x + 0x20 : (char) x;
		}
	}

	//if (IgnorePaths)
	//	deletePathFromFilename(entryFileName);

	entry.filePathHash = hashStringSimple(entryFileName.c_str());


	S32 index =  -1;

	std::vector<SPackResFileEntry>::const_iterator ite = std::lower_bound(m_fileList.begin(), m_fileList.end(), entry);
	for ( ; ite != m_fileList.end() && *ite == entry; ++ite)
	{
		const SPackResFileEntry &foundEntry = *ite;
		//Verify the name whether correct, because hash value may be same but string not same
		if (
#ifdef WIN32
	#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)
			_stricmp(entryFileName.c_str(), foundEntry.fileName) == 0
	#else
			glitch::core::stricmp(entryFileName.c_str(), foundEntry.fileName) == 0
	#endif
#else
			stricmp(entryFileName.c_str(), foundEntry.fileName) == 0
#endif
			)
		{
			// move forward length of extra field.
			index =	 ite - m_fileList.begin();
			break;
		}

	}

	return index;
}

bool 
CPackResReader::isValid(IReadResFile* file)
{
	U32 header = 0;
	int pos = file->getPos();
	file->read(&header, 4);
	file->seek(pos);
	return (header == 0x04034b50 || header == 0x504d4247 );
}

bool 
CPackResReader::isValid(const char * filename)
{
	IReadResFile *readFile = createReadFile(filename);
	bool validation = isValid(readFile);
	readFile->drop();
	return validation;
}

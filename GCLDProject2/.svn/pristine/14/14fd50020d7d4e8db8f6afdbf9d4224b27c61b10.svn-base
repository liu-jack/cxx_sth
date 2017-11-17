//#include "preHeader.h"
//#include "stdafx.h"
#include "PackPatchReader.h"


CPackPatchReader::CPackPatchReader(const char* filename, bool ignoreCase, bool ignorePaths)
	: CPackResReader(filename, ignoreCase, ignorePaths)
{
}

CPackPatchReader::CPackPatchReader(IReadResFile* file, bool ignoreCase, bool ignorePaths)
	: CPackResReader(file, ignoreCase, ignorePaths)
{
}

CPackPatchReader::~CPackPatchReader(void)
{
	for (U32 i = 0; i < PackPatchFiles.size(); ++i)
	{
		delete PackPatchFiles[i];
	}
	PackPatchFiles.clear();
}

bool CPackPatchReader::addPackPatchFile(const char* filename, bool ignoreCase/* = true*/, bool ignorePaths /*= true*/)
{
	CPackResReader* zr = 0;
	IReadResFile* m_file = createReadFile(filename);
	if (m_file)
	{
		zr = new CPackResReader(m_file, ignoreCase, ignorePaths);
		if (zr)
		{
			PackPatchFiles.push_back(zr);
			
			//Update Index in the Main Zip
			int curPatchNo = PackPatchFiles.size() - 1;
			int fileEntryNb = zr->getFileCount();
			bool needSort = false;


			for(int i=0; i<fileEntryNb; ++i)
			{
				const SPackResFileEntry * fileInfo =  zr->getFileInfo(i);
				if(fileInfo == NULL)
					continue;
				S32 res = this->findFile(fileInfo->fileName);
				if(res < 0)
				{
					needSort = true;
					res = m_fileList.size();
					m_fileList.push_back(*fileInfo);
				}
				this->m_fileList[res].fileDataPosition = 0x80000000 | (curPatchNo << 16) | i;

			}

			if(needSort)
			{
				//core::heapsort(&m_fileList[0], m_fileList.size());
				std::sort(m_fileList.begin(), m_fileList.end());
			}


		}

		m_file->drop();
	}
	//#ifdef _DEBUG
	//else
	//	os::Printer::log("Could not open m_file. Zipfile patch not added", filename, ELL_ERROR);
	//#endif

//	GLITCH_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
	return (zr != 0);
}


IReadResFile* CPackPatchReader::openFile(const char* filename)
{
	S32 index = findFile(filename);
	if (index != -1)
	{
		S32 fileDataPosition = m_fileList[index].fileDataPosition;
		if((fileDataPosition & 0x80000000)==0)
		{
			return CPackResReader::openFile(index);
		}
		else
		{
			int zipPatchIndex = (fileDataPosition & 0x7FFF0000) >> 16;
			int zipLocalIndex = fileDataPosition & 0xFFFF;

			return PackPatchFiles[zipPatchIndex]->openFile(zipLocalIndex);
		}
	}
	return NULL;
}


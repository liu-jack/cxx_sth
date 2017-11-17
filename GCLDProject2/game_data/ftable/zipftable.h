#pragma once
////////////////////////////////////////////////////////////////////
/*
File Format:
	Header
	indexs
	col Formats
	fixed entrys 
	std::string table
*/
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <map>
#include <string>

#include "def/TypeDef.h"
#include "../data_reader/IReadResFile.h"

class CZipFTable
{
public:
	//struct Header
	//{
	//	uint8		FLAG[8]; 
	//	uint32		nVersion;
	//	uint32		nRows;
	//	uint32		nCols;		
	//	uint32		nOffsetIndex;
	//	uint32		nOffsetFormat;
	//	uint32		nOffsetEntry;
	//	uint32		nEntrySize;
	//	uint32		nOffsetStrTable;
	//	uint32		nLengthStrTable;
	//};

private:
	mutable IReadResFile *m_resFile;

	FILE* m_file;	//For Write Only

	FTableHeader m_header;
	uint32	m_nFileSize;

public:
	CZipFTable();
	~CZipFTable();

	bool OpenFile(const char* pszFileName);
	bool Close();
	bool IsFileOpen() const;
	
	const FTableHeader&	GetHeader() const {
		return m_header;
	}
	const std::string&	GetFormat() const {
		return m_strFormat;
	}
	//reader
protected:
	bool ReadHeader();
	bool ReadIndex();
	bool ReadFormat();
	
public:
	bool LoadEntryBySN(uint32 entrySN,char* buf) const;
	bool LoadEntryById(uint32 entryId,char* buf) const;
	bool ReadString(uint32 nOffset, uint16 nLen,std::string& strOut)  const;
	
	//writer
private:
	//id,serial number
	std::vector< uint32 >		m_index;
	std::string					m_strFormat;
	uint32						m_iCurEntry;
	uint32						m_iCurString;
public:
	bool CreateFTable(const char* pszFileName, uint32 nRows, uint32 nCols, uint32 nEntrySize, const char* pszFormat);
	bool BeginWriteEntry();
	uint32 WriteEntry(uint32 nId, const char* pEntry);
	bool EndWriteEntry();
	uint32 WriteString(const char* pszString, uint32 nLen);
};

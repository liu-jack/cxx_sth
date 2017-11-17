
#include <vector>

#include "zipftable.h"
#include "../data_reader/CReadResFile.h"
#include "../data_reader/PackPatchReader.h"


#define USE_MAP_INDEX 0

CZipFTable::CZipFTable()
: m_resFile(NULL)
{
	m_file = NULL;
	memset( (void*)&m_header,0,sizeof(m_header) );
}

CZipFTable::~CZipFTable()
{

}

bool CZipFTable::OpenFile(const char* pszFileName)
{
	Close();
	assert(m_resFile==NULL); //Open A Same File Twice or more
	if(m_resFile != NULL)
	{
		return true;
	}

	m_resFile = createReadFile(pszFileName);
	//m_file = fopen(pszFileName, "rb");

	if ( !m_resFile )
	{
		return false;
	}
	m_nFileSize =m_resFile->getSize() /*ftell(m_file)*/;
	bool isOk = ReadHeader();
	if ( !isOk )
	{
		return false;
	}
	isOk = ReadFormat();
	if ( !isOk )
	{
		return false;
	}

	if( m_header.nRows == 0 )
	{
		return true;
	}
	isOk = ReadIndex();
	if ( !isOk )
	{
		return false;
	}


	//fseek(m_file,0,SEEK_END);
	return true;
}

bool CZipFTable::IsFileOpen() const
{
	return (m_resFile != NULL);
}

bool CZipFTable::Close()
{
	if(m_resFile)
	{
		m_resFile->drop();
		m_resFile = NULL;
	}
	if ( m_file )
	{
		fclose( m_file );
	}
	//memset( (void*)&m_header,0,sizeof(m_header) );
	m_file = NULL;
	return true;
}

bool CZipFTable::ReadHeader()
{
	m_resFile->seek(0);
	int nLen = m_resFile->read(&m_header,sizeof(m_header));

	//fseek(m_file,0,SEEK_SET);
	//int nLen = fread(&m_header,sizeof(m_header),1,m_file);
	if ( nLen <= 0 )
	{
		return false;
	}
	return true;
}

bool CZipFTable::ReadIndex()
{
#if USE_MAP_INDEX
	std::vector<uint32> buf(m_header.nRows,0);
	fseek(m_file,m_header.nOffsetIndex,SEEK_SET);
	fread(&buf[0],sizeof(uint32),m_header.nRows,m_file);

	for(uint32 iIdx = 0; iIdx < m_header.nRows; iIdx++ )
	{
		m_index.insert( std::make_pair( buf[iIdx],iIdx ) );
	}
#else
	//std::vector<uint32> buf(m_header.nRows,0);
	m_index.resize(m_header.nRows, 0);
	m_resFile->seek(m_header.nOffsetIndex);
	m_resFile->read(&m_index[0],sizeof(uint32)* m_header.nRows);
#endif
	//fseek(m_file,m_header.nOffsetIndex,SEEK_SET);
	//fread(&buf[0],sizeof(uint32),m_header.nRows,m_file);

	//for(uint32 iIdx = 0; iIdx < m_header.nRows; iIdx++ )
	//{
	//	m_index.insert( buf[iIdx],iIdx ) );
	//}
	return true;
}

bool CZipFTable::ReadFormat()
{

	m_strFormat.resize( m_header.nCols + 1);
	m_resFile->seek(m_header.nOffsetFormat);
	m_resFile->read(&m_strFormat[0],sizeof(char)* (m_header.nCols+1));

	//fseek(m_file,m_header.nOffsetFormat,SEEK_SET);
	//fread(&m_strFormat[0],sizeof(char),m_header.nCols+1,m_file);
	return true;
}

bool CZipFTable::LoadEntryBySN(uint32 entrySN,char* buf) const
{
	uint32 nOffset = m_header.nOffsetEntry + entrySN * m_header.nEntrySize;
	m_resFile->seek(nOffset);
	m_resFile->read(buf,m_header.nEntrySize);
	
	//fseek(m_file,nOffset,SEEK_SET);
	//fread(buf,m_header.nEntrySize,1,m_file);
	return true;
}

bool CZipFTable::LoadEntryById(uint32 entryIdx,char* buf) const
{
#if USE_MAP_INDEX
	std::map< uint32, uint32>::const_iterator iter = m_index.find(entryIdx);
	if ( iter == m_index.end() )
	{
		return false;
	}

	uint32 nEntrySerialNumber = iter->second;
	return LoadEntryBySN( nEntrySerialNumber,buf );
#else
	std::vector<uint32>::const_iterator iter = std::lower_bound(m_index.begin(), m_index.end(), entryIdx);
	if(iter == m_index.end() || *iter != entryIdx)
	{
		return false;
	}
	return LoadEntryBySN(iter - m_index.begin(), buf);

#endif
}


bool CZipFTable::ReadString(uint32 nOffset, uint16 nLen,std::string& strOut) const
{
	if ( nOffset+nLen > m_nFileSize )
		return false;

	if ( nLen == 0 ) return true;

	strOut.resize(nLen);
	m_resFile->seek(nOffset);
	m_resFile->read(&strOut[0],nLen);
	//fseek(m_file,nOffset  ,SEEK_SET);
	//fread(&strOut[0],nLen,1,m_file);	
	return true;
}

bool CZipFTable::CreateFTable(const char* pszFileName, uint32 nRows, uint32 nCols, uint32 nEntrySize, const char* pszFormat)
{
//	Close();
	m_file = fopen(pszFileName,"wb");
	if ( !m_file )
		return false;

	uint32 offset = 0;
	FTableHeader& header = m_header;
	strcpy( (char*)header.FLAG,TABLE_STR_FLAG);
	header.nVersion			= TABLE_VERSION;
	header.nRows			= nRows;
	header.nCols			= nCols;

	offset += sizeof(header);
	header.nOffsetIndex		= offset;

	offset += sizeof(uint32) * nRows;
	header.nOffsetFormat	= offset;

	offset += sizeof(char) * nCols + 1;
	header.nOffsetEntry		= offset;
	header.nEntrySize		= nEntrySize;

	offset += nEntrySize * nRows;
	header.nOffsetStrTable	= offset;
	
	fwrite(&header,sizeof(header),1,m_file );

	fseek(m_file,header.nOffsetFormat,SEEK_SET);
	fwrite(pszFormat,sizeof(char),nCols+1,m_file);
	return true;
}

bool CZipFTable::BeginWriteEntry()
{
#if USE_MAP_INDEX
	m_index.clear();
	m_iCurEntry		= 0;
	m_iCurString	= m_header.nOffsetStrTable;
	return true;
#else
	return false;
#endif
}

uint32 CZipFTable::WriteEntry(uint32 nId, const char* pEntry)
{
#if USE_MAP_INDEX
	uint32 nEntryOffset = m_header.nOffsetEntry+ m_iCurEntry * m_header.nEntrySize;
	fseek(m_file,nEntryOffset,SEEK_SET);
	fwrite(pEntry, m_header.nEntrySize, 1, m_file);

	m_index.insert( std::make_pair(nId,m_iCurEntry) );

	m_iCurEntry++;
	return m_iCurEntry;
#else
	return false;
#endif
}

bool CZipFTable::EndWriteEntry()
{
#if USE_MAP_INDEX
	uint32 nEntryCount = m_iCurEntry;
	std::vector< uint32 > buf(nEntryCount,0);

	std::map< uint32,uint32 >::iterator iter = m_index.begin();
	while ( iter != m_index.end() )
	{
		if ( iter->second < nEntryCount ) 
		{
			buf[iter->second] = iter->first;
		}
		else{
			return false;
		}
		iter++;
	}
	fflush(m_file);
	if ( !m_index.empty() )
	{
		fseek(m_file, m_header.nOffsetIndex, SEEK_SET);
		fwrite( &buf[0], sizeof(uint32)*nEntryCount, 1, m_file );
	}
	
	m_index.clear();
	m_iCurEntry = 0;
	Close();
	return true;
#else
	return false;
#endif
}

uint32 CZipFTable::WriteString(const char* pszString, uint32 nLen)
{
	if ( !m_file )
		return 0;

	fseek(m_file, m_iCurString, SEEK_SET);
	fwrite(pszString, sizeof(char), nLen+1, m_file);
	uint32 offset = m_iCurString ;
	m_iCurString += (nLen + 1);
	return offset;
}


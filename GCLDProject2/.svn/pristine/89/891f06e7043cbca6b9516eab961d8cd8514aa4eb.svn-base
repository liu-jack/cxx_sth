//#include "preHeader.h"
//#include "stdafx.h"
#include "ResFileStream.h"
#include "IReadResFile.h"

CResFileStream::CResFileStream(IReadResFile* pFile)
{
	m_pReadFile = pFile;
//	m_pWriteFile = NULL;
	m_readError =false;
	if(m_pReadFile)
	{
		m_pReadFile->grab();
	}
}

CResFileStream::~CResFileStream()
{
	if( m_pReadFile )
	{
		m_pReadFile->drop();
		m_pReadFile = NULL;
	}

	//if( m_pWriteFile )
	//{
	//	m_pWriteFile->drop();
	//	m_pWriteFile = NULL;
	//}

	m_readError = false;
}


void CResFileStream::seek(int offset)
{
	if( m_pReadFile )
	{
		bool b = m_pReadFile->seek((long)offset);
		if( b )
		{
			return;
		}
	}
	//else if( m_pWriteFile )
	//{
	//	m_pWriteFile->seek((long)offset);
	//}
}
int	CResFileStream::tell() const
{
	if( m_pReadFile )
	{
		return m_pReadFile->getPos();
	}
	//else if( m_pWriteFile )
	//{
	//	return m_pWriteFile->getPos();
	//}
	return 0;
}

int	CResFileStream::size() const
{
	if( m_pReadFile )
	{
		return m_pReadFile->getSize();
	}
	//else if( m_pWriteFile )
	//{
	//	return m_pWriteFile->getPos();
	//}
	return 0;
}

bool CResFileStream::canRead() const
{
	if( m_pReadFile )
	{
		return true;
	}
	//else if( m_pWriteFile )
	//{
	//	return false;
	//}
	return false;
}
bool CResFileStream::canWrite() const
{
	if( m_pReadFile )
	{
		return false;
	}
	//else if( m_pWriteFile )
	//{
	//	return true;
	//}
	return false;
}

int CResFileStream::read(U8* data, int len)
{
	if( m_pReadFile )
	{
		int32 num = m_pReadFile->read( data,(uint32)len);
		if( num == 0)
		{
			m_readError = true;
		}
		return num;
	}
	return 0;
}

//int CFileStream::write(const U8* data, int len)
//{
//	if( m_pWriteFile )
//	{
//		return m_pWriteFile->write( data, (uint32)len);
//	}
//	return 0;
//}


void CResFileStream::setInputFileObject(IReadResFile* pFile)
{
	m_pReadFile = pFile;
	m_pReadFile->grab();
	
}
//void CFileStream::setOutputFileObject(io::IWriteFile *pFile)
//{
//	m_pWriteFile = pFile;
//}
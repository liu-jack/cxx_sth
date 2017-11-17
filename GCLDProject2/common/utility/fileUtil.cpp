

#include "fileUtil.h"

/*
* ������������
*/
CFileOperator::CFileOperator( void )
{
}

CFileOperator::~CFileOperator( void )
{
}

/*
* ��ȡ������
*/
CFileOperator& CFileOperator::getSingleInstance( void )
{
	static CFileOperator singleFileOperator ;
	return singleFileOperator ;
}


/*
* �����ļ���Ŀ¼ע��
* @ strDir Ŀ¼( ע��·������ '/' ��Ϊ�ָ����� )
*/
bool CFileOperator::createFileFolder( std::string &strDir )
{
	//��ĩβ��/
	if( *strDir.rbegin() != '/' )
	{
		strDir.append( "/" ) ;
	}

	size_t iRet = static_cast<size_t>( -1 ) ;

	// ����Ŀ¼
	std::string tempPath ;
	size_t begIndex = strDir.find_first_of( '/', 0 ) ;

	while( begIndex != std::string::npos  )
	{
		tempPath = strDir.substr( 0, begIndex ) ; // �����б����� a/b/c/d... ������

		//���������,����
		iRet = ACCESS( tempPath.c_str() ,0 ) ;
		if( iRet != 0 )
		{
			iRet = MKDIR( tempPath.c_str() );
			if( iRet != 0 )
			{
				return false;
			} 
		}

		++begIndex ;
		begIndex = strDir.find_first_of( '/', begIndex ) ;
	}

	return true;
}


/*
* ��һ���ļ�
*/
std::fstream& CFileOperator::openFile( const std::string &path, std::fstream &file, std::fstream::open_mode openMode )
{
	if( !file.is_open() )
	{
		file.clear() ;
		file.open( path.c_str(), static_cast<std::ios_base::openmode>( openMode ) ) ;
	}
	return file ;
}

/*
* �ر�һ���ļ�
*/
std::fstream& CFileOperator::closeFile( std::fstream &file )
{
	if( file.is_open() )
	{
		file.flush() ;
		file.close() ;
		file.clear() ;
	}
	return file ;
}

/*
* �ļ����Ϊ
*/
bool CFileOperator::saveAs( const std::string &orignFilePath, const std::string &destFilePath )
{
	std::fstream inFile ;
	if( !sFile.openFile( orignFilePath, inFile, std::fstream::in | std::fstream::binary ) )
	{
		return false ;
	}

	std::fstream outFile ;
	if( !openFile( destFilePath, outFile, std::fstream::out | std::fstream::binary | std::fstream::trunc ) )
	{
		return false ;
	}

	//std::copy( istream_iterator<char>( configFile ), istream_iterator<char>(), ostream_iterator<char>( outFile ) ) ;
	std::string str ;
	while( std::getline( inFile, str, '\r' ) )
	{
		outFile << str << '\r' ;
	}

	closeFile( outFile ) ;

	return true ;
}

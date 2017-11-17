

#include "fileUtil.h"

/*
* 构造析构函数
*/
CFileOperator::CFileOperator( void )
{
}

CFileOperator::~CFileOperator( void )
{
}

/*
* 获取单例类
*/
CFileOperator& CFileOperator::getSingleInstance( void )
{
	static CFileOperator singleFileOperator ;
	return singleFileOperator ;
}


/*
* 创建文件夹目录注意
* @ strDir 目录( 注意路径中以 '/' 作为分隔符号 )
*/
bool CFileOperator::createFileFolder( std::string &strDir )
{
	//在末尾加/
	if( *strDir.rbegin() != '/' )
	{
		strDir.append( "/" ) ;
	}

	size_t iRet = static_cast<size_t>( -1 ) ;

	// 创建目录
	std::string tempPath ;
	size_t begIndex = strDir.find_first_of( '/', 0 ) ;

	while( begIndex != std::string::npos  )
	{
		tempPath = strDir.substr( 0, begIndex ) ; // 过程中保持以 a/b/c/d... 层层访问

		//如果不存在,创建
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
* 打开一个文件
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
* 关闭一个文件
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
* 文件另存为
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

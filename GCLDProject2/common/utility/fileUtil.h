/*
* Copyright (c) 2013--2015,www.521g.cn 
* All rights reserved.
* 
* �ļ����ƣ�fileUtil.h
* ժ    Ҫ���ı�����������
* 
* ��ǰ�汾��1.0.0.1
* ��    �ߣ�CaiBingjie
* ������ڣ�2013��5��3��
*
*/

#ifndef CX_FILE_UTIL_H__
#define CX_FILE_UTIL_H__

// Ŀ¼������Ҫ�õ���ͷ�ļ�
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

// C++ ��Ŀͷ�ļ�
#include <string>
#include <fstream>



class CFileOperator
{
protected:
	CFileOperator( void ) ;

public:
	~CFileOperator( void ) ;

	/*
	 * ��ȡ������
	 */
	static CFileOperator& getSingleInstance( void ) ;

	/*
	* �����ļ���Ŀ¼ע��
	* @ strDir Ŀ¼( ע��·������ '/' ��Ϊ�ָ����� )
	*/
	bool createFileFolder( std::string &strDir ) ;

	/*
	* ��һ���ļ�
	*/
	std::fstream& openFile( const std::string &path, std::fstream &file, std::fstream::open_mode openMode ) ;

	/*
	* �ر�һ���ļ�
	*/
	std::fstream& closeFile( std::fstream &file ) ;

	/*
	* �ļ����Ϊ
	*/
	bool saveAs( const std::string &orignFilePath, const std::string &destFilePath ) ;
} ;

// ������ȡ��
#define sFile CFileOperator::getSingleInstance()

#endif

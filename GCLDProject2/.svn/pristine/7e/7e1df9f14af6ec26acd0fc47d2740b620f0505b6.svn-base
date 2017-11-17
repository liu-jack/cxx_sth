/*
* Copyright (c) 2013--2015,www.521g.cn 
* All rights reserved.
* 
* 文件名称：fileUtil.h
* 摘    要：文本操作工具类
* 
* 当前版本：1.0.0.1
* 作    者：CaiBingjie
* 完成日期：2013年5月3日
*
*/

#ifndef CX_FILE_UTIL_H__
#define CX_FILE_UTIL_H__

// 目录常见需要用到的头文件
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

// C++ 项目头文件
#include <string>
#include <fstream>



class CFileOperator
{
protected:
	CFileOperator( void ) ;

public:
	~CFileOperator( void ) ;

	/*
	 * 获取单例类
	 */
	static CFileOperator& getSingleInstance( void ) ;

	/*
	* 创建文件夹目录注意
	* @ strDir 目录( 注意路径中以 '/' 作为分隔符号 )
	*/
	bool createFileFolder( std::string &strDir ) ;

	/*
	* 打开一个文件
	*/
	std::fstream& openFile( const std::string &path, std::fstream &file, std::fstream::open_mode openMode ) ;

	/*
	* 关闭一个文件
	*/
	std::fstream& closeFile( std::fstream &file ) ;

	/*
	* 文件另存为
	*/
	bool saveAs( const std::string &orignFilePath, const std::string &destFilePath ) ;
} ;

// 单例获取简化
#define sFile CFileOperator::getSingleInstance()

#endif

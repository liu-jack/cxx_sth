#ifndef MD5_TOOL_H__
#define MD5_TOOL_H__

#include <vector>
#include <string>
#include <algorithm>

#include "md5.h"

struct Md5Tool
{
public:
	template< class INT_TYPE >
	static void CalculateKey( std::string &key, std::vector<INT_TYPE> &args )
	{
		if( args.empty() )
		{
			return ;
		}

		const uint32 elementSize =sizeof( INT_TYPE ) ;
		const uint32 arraySize   = elementSize * args.size() ;

		std::string str ;
		str.reserve( arraySize ) ;

		char *pArgBeg =reinterpret_cast<char*>( &args[ 0 ] ) ;
		char *pArgEnd =pArgBeg + arraySize - 1 ;

		char currentChar ='\0' ;
		char replaceChar ='\0' ;
		char replaceWith ='*' ;
		while( pArgBeg <= pArgEnd )
		{
			currentChar = *pArgBeg == replaceChar ? replaceWith : *pArgBeg  ;
			str.push_back( currentChar ) ;
			++pArgBeg ;

			if( pArgBeg <= pArgEnd )
			{
				currentChar = *pArgEnd == replaceChar ? replaceWith : *pArgEnd  ;
				str.push_back( currentChar ) ;
				--pArgEnd ;
			}
		}

		ASSERT( str.size() == arraySize ) ;
		str.push_back( '\0' ) ;

		MD5 md5 ;
		char *pRet =md5.MD5String( str ) ;
		key.assign( pRet ) ;

		::free( pRet ) ;
	}

	static void CalculateKey( std::string &key, std::vector<std::string> &args )
	{
		std::string strTmp ;
		for( std::vector<std::string>::iterator itr =args.begin(); itr != args.end(); ++itr )
		{
			strTmp.append( *itr ) ;
		}

		MD5 md5 ;
		char *pRet =md5.MD5String( strTmp ) ;
		key.assign( pRet ) ;

		::free( pRet ) ;
	}

	static void CalculateKey( std::string &key, const std::string &args )
	{
		MD5 md5 ;
		char *pRet =md5.MD5String( args ) ;
		key.assign( pRet ) ;

		::free( pRet ) ;
	}
} ;

#endif
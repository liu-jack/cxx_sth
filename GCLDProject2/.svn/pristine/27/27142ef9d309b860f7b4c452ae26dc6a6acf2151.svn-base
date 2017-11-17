#ifndef SHA1TOOL_H__
#define SHA1TOOL_H__

//#include <vector>
#include <string>
//#include <algorithm>

#include "HMAC_SHA1.h"

struct SHA1Tool
{
public:
	static void CalculateKey( std::string &key, std::string &srcStr, std::string &srcKey )
	{
		CHMAC_SHA1 sha1;
		BYTE shaStr[100] = {'\0'};
		string tmpSrc(srcStr);
		string tmpKey(srcKey);
		sha1.HMAC_SHA1(const_cast<unsigned char *>((const unsigned char *)tmpSrc.c_str()),strlen(tmpSrc.c_str()),const_cast<unsigned char *>((const unsigned char *)tmpKey.c_str()),strlen(tmpKey.c_str()),shaStr);
		key.assign( (char*)shaStr ) ;
	}
} ;

#endif
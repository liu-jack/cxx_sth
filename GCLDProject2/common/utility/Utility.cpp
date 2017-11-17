
#include "Utility.h"


#include <ctype.h>
#include <string.h>
#include<sstream>
#include "Base.h"
namespace Utility
{
	const char digits[] = "9876543210123456789";
	const char* zero = digits + 9;
	BOOST_STATIC_ASSERT(sizeof(digits) == 20);

	const char digitsHex[] = "0123456789ABCDEF";
	BOOST_STATIC_ASSERT(sizeof(digitsHex) == 17);

	void GetRandStr( std::string &pStart, uint32 length )
	{
		char* ss = new char[length+1];
		memset(ss,0,(length+1)*sizeof(char));
		srand((unsigned int)time((time_t *)NULL));
		for(int i=0;i< length;i++){
			ss[i] = digitsHex[(rand()%16)];
		}
		pStart.append(ss);
		delete []ss;
		ss = NULL;
	}

	bool GetOneUInt32Value( char *&pStart, uint32 &value )
{
	return GetIntValue( pStart, value ) ;
}

bool GetOneUInt64Value( char *&pStart, uint64 &value )
{
	return GetIntValue( pStart, value ) ;
}

void PacketItemsToString(string& str,const uint32 type,const uint32 count)
{
#define MAX_ARRAY_SIZE 4096
	char buf_type[MAX_ARRAY_SIZE] = {0}, buf_count[MAX_ARRAY_SIZE] = {0};
	Utility::convert(buf_type,type);
	Utility::convert(buf_count,count);
	if(str.empty())
	{
		str +=  string(buf_type) + "|" + string(buf_count);
	}
	else
	{
		str += "," + string(buf_type) + "|" + string(buf_count);
	}
}
void PacketItemsToString(string& str,const IntPair& pair)
{
	PacketItemsToString(str,pair.first,pair.second);
}

void PacketItemsToString(string& str,const IntPairVec& vec)
{
	for(IntPairVec::const_iterator iter = vec.begin();iter!= vec.end();++iter)
	{
		PacketItemsToString(str,*iter);
	}
}

// 在一段以逗号间隔的字符串里面获取一字符串, 且指针移动
bool GetOneStringValue( char *&pStart, string &sValue, char delimiter /*=','*/ )
{
	if( pStart == NULL || *pStart == '\0' )
	{
		return false ;
	}

	while( *pStart == delimiter )
	{
		++pStart ;
	}

	//while( !isalnum( *pStart ) )
	//{
	//	++pStart ;
	//}

	char *pEnd = strchr(pStart, delimiter );
	if( pEnd == NULL )
	{
		if( pStart != pEnd && pStart != '\0' )
		{
			sValue.assign( pStart ) ;
			pStart =pEnd ;
			return true ;
		}
		return false ;
	}

	*pEnd ='\0';
	sValue.assign( pStart ) ;
	pStart = ++pEnd ;
	return true ;
}

bool GetStringValue( std::vector<int>& retVec,string& outStr, const char split )
{
	int nSize = retVec.size();
   if (nSize > 0)
   {
	   for (int i=0; i< nSize; i++)
	   {
		   char tmp[16] = {0};
		   snprintf( tmp, 16, "%d", retVec[i]);
		   outStr.append(tmp);
		   if ((i+1) < nSize)
		   {
			  outStr.append(1,split);
		   }
	   }
	   return true;
   }
   return false;
}

bool GetStringValueForSet( std::set<int>& retSet,string& outStr, const char split )
{
	for (std::set<int>::iterator additer = retSet.begin(); additer!= retSet.end();)
	{
		int value = (*additer);
		char tmp[16] = {0};
		snprintf( tmp, 16, "%d", value);
		outStr.append(tmp);
		++additer;
		if (additer != retSet.end())
		{
			outStr.append(1,split);
		}
	}
	return ((int)retSet.size()) > 0;
}

void SplitStr(const string& str, std::vector<string>& retVec, const char split)
{
    int beginIdx = 0;
    int retIdx = str.find_first_of(split, beginIdx);
    while (retIdx != -1)
    {
        if (retIdx > beginIdx)
        {
            retVec.push_back(str.substr(beginIdx, retIdx - beginIdx));
        }
        beginIdx = retIdx + 1;
        retIdx = str.find_first_of(split, beginIdx);
    }
    if (beginIdx < (int)str.length())
    {
        retVec.push_back(str.substr(beginIdx, str.length() - beginIdx));
    }
}
void SplitStr(const std::string& str, std::vector<int>& retVec, const char split)
{
    int beginIdx = 0;
    int retIdx = str.find_first_of(split, beginIdx);
    while (retIdx != -1)
    {
        if (retIdx > beginIdx)
        {
            retVec.push_back(atoi(str.substr(beginIdx, retIdx - beginIdx).c_str()));
        }
        beginIdx = retIdx + 1;
        retIdx = str.find_first_of(split, beginIdx);
    }
    if (beginIdx < (int)str.length())
    {
        retVec.push_back(atoi(str.substr(beginIdx, str.length() - beginIdx).c_str()));
    }
}
void SplitStr(const std::string& str, IntPairVec& retVec) // "(2|3)(7|4)(9|6)"
{
    std::pair<int, int> tmp;
    int beginIdx = 0;
    int retIdx = str.find_first_of('(', beginIdx);
    while (retIdx != -1)
    {
        beginIdx = retIdx + 1;
        retIdx = str.find_first_of('|', beginIdx);
        tmp.first = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());

        beginIdx = retIdx + 1;
        retIdx = str.find_first_of(')', beginIdx);
        tmp.second = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());

        retVec.push_back(tmp);
        retIdx = str.find_first_of('(', beginIdx);
    }
}

void SplitStr( const string& str, std::set<uint64>& retSet, const char split )
{
	int beginIdx = 0;
	int retIdx = str.find_first_of(split, beginIdx);
	while (retIdx != -1)
	{
		if (retIdx > beginIdx)
		{
			std::stringstream time_value;
			time_value << str.substr(beginIdx, retIdx - beginIdx);
			uint64 relValue;
			time_value >> relValue;
			retSet.insert(relValue);
		}
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(split, beginIdx);
	}
	if (beginIdx < (int)str.length())
	{
		std::stringstream time_value;
		time_value << str.substr(beginIdx, str.length() - beginIdx);
		uint64 relValue;
		time_value >> relValue;
		retSet.insert(relValue);
	}
}

void SplitStr( const string& str, std::set<int>& retSet, const char split )
{
	int beginIdx = 0;
	int retIdx = str.find_first_of(split, beginIdx);
	while (retIdx != -1)
	{
		if (retIdx > beginIdx)
		{
			retSet.insert(atoi(str.substr(beginIdx, retIdx - beginIdx).c_str()));
		}
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(split, beginIdx);
	}
	if (beginIdx < (int)str.length())
	{
		retSet.insert(atoi(str.substr(beginIdx, str.length() - beginIdx).c_str()));
	}
}
///分割到intPair中
void SplitStr(const string& str,IntPair& p,const char split)
{
	int beginIdx = 0;
	int retIdx = str.find_first_of(split, beginIdx);
	if(retIdx != -1)
	{
		if (retIdx > beginIdx)
		{
			p.first = (atoi(str.substr(beginIdx, retIdx - beginIdx).c_str()));
		}
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(split, beginIdx);
	}
	if (beginIdx < (int)str.length())
	{
		p.second = (atoi(str.substr(beginIdx, str.length() - beginIdx).c_str()));
	}
}


void SplitStr(const string& str,VecIntPairvec& retvec)
{
	int beginIdx = 0;
	int retIdx = str.find_first_of('(', beginIdx);
	while (retIdx != -1)
	{
		IntPairVec vec;
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(')', beginIdx);
		SplitStr2(str.substr(beginIdx, retIdx - beginIdx),vec);
		retIdx = str.find_first_of('(', beginIdx);
		retvec.push_back(vec);
	}
}

void SplitStr2(const std::string& str, IntPairVec& retVec) // "2|3,7|4,9|6"
{
    std::pair<int, int> tmp;
    int beginIdx = 0;
    int retIdx = str.find_first_of('|', beginIdx);
    while (retIdx != -1)
    {
        tmp.first = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());

        beginIdx = retIdx + 1;
        retIdx = str.find_first_of(',', beginIdx);
        if (retIdx == -1)
		{
			tmp.second = atoi(str.substr(beginIdx).c_str());
			retVec.push_back(tmp);
			break;
		}
        tmp.second = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
        retVec.push_back(tmp);

        beginIdx = retIdx + 1;
        retIdx = str.find_first_of('|', beginIdx);
    }
}


void SplitStr3( const std::string& str, IntV3Vec& retVec )// "2|3|5,7|4|7,9|6|6"
{
	IntV3 tmp;
	int beginIdx = 0;
	int retIdx = str.find_first_of('|', beginIdx);
	while (retIdx != -1)
	{
		tmp.first = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
		tmp.second = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(',', beginIdx);
		if (retIdx == -1)
		{
			tmp.third = str.substr(beginIdx);
			retVec.push_back(tmp);
			break;
		}
		tmp.third = str.substr(beginIdx, retIdx - beginIdx);
		retVec.push_back(tmp);
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
	}
}

void SplitStr4(const std::string& str, IntV4Vec& retVec)// "2|3|5|3,7|4|7|4,9|6|6|5"
{
	IntV4 tmp;
	int beginIdx = 0;
	int retIdx = str.find_first_of('|', beginIdx);
	while (retIdx != -1)
	{
		tmp.first = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
		tmp.second = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
		tmp.third = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(',', beginIdx);
		if (retIdx == -1)
		{
			tmp.fourth = atoi(str.substr(beginIdx).c_str());
			retVec.push_back(tmp);
			break;
		}
		tmp.fourth = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		retVec.push_back(tmp);
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
	}
}

void SplitStrISpair( const std::string& str, ISpairMap& retMap )
{
	StringPair tmp;
	int beginIdx = 0;
	int retIdx = str.find_first_of('|', beginIdx);
	while (retIdx != -1)
	{
		int key = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
		tmp.first = str.substr(beginIdx, retIdx - beginIdx);
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(',', beginIdx);
		if (retIdx == -1)
		{
			tmp.second = str.substr(beginIdx);
			retMap[key] = tmp;
			break;
		}
		tmp.second = str.substr(beginIdx, retIdx - beginIdx);
		retMap[key] = tmp;
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
	}
}

void SplitItemStr( const std::string& str, ItemID2Count& retMap )// "2|3,7|4,9|6"
{
	ItemId itemId;
	int beginIdx = 0;
	int retIdx = str.find_first_of('|', beginIdx);
	while (retIdx != -1)
	{
		itemId = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of(',', beginIdx);
		if (retIdx == -1)
		{
			retMap[itemId] = atoi(str.substr(beginIdx).c_str());
			break;
		}
		retMap[itemId] = atoi(str.substr(beginIdx, retIdx - beginIdx).c_str());
		beginIdx = retIdx + 1;
		retIdx = str.find_first_of('|', beginIdx);
	}
}

void SplitAttrLv(const std::string& str, IntPairVec& retVec) // "2_3_7_4_9_6"
{
	std::pair<int, int> tmp;
	std::vector<int> atrrLv;
	SplitStr(str, atrrLv, '_');
	int nPiar = (int)atrrLv.size() / 2;
	for (int i=0; i< nPiar; i++)
	{
		tmp.first = atrrLv[i*2];
		tmp.second = atrrLv[i*2 + 1];
		retVec.push_back(tmp);
	}
}

// 将 number 位为 1 的单独成为一个值保存到队列中
void GetPerBitNumberInInteger( const uint32 &number, std::deque<uint32> &deque )
{
	uint32 bitMark = 0x1 ;
	for( uint32 index =0; index < 32; ++index )
	{
		bitMark = 0x1 << index ;
		if( number & bitMark )
		{
			deque.push_back( bitMark ) ;
		}
	}
}

void RightShift( std::string& str, unsigned count, char ch /*= ' '*/ )
{
    string::size_type size = str.size();
    str.resize( size + count, ch);
    string::reverse_iterator it1 = str.rbegin();
    string::reverse_iterator it2 = it1 + count;
    for ( string::size_type i = 0; i < size; ++i)
    {
        *it1++ = *it2++;
    }
    for( ;it1 != str.rend(); ++it1)
    {
        *it1 = ch;
    } 
}

void FormatString( std::string &sql, const char* szQueryString, ... )
{
    char buffer[512];
    va_list vlist;
    va_start(vlist, szQueryString);
    int retValut =vsnprintf(buffer, 512, szQueryString, vlist);
    va_end(vlist);

    ASSERT( retValut >= 0 ) ;

    sql.assign( buffer ) ;
}

size_t converHex(char buf[],uintptr_t value)
{
	uintptr_t i = value;
	char* p = buf;

	do 
	{
		int lsd = static_cast<int>(i %16);
		i /= 16;
		*p++ = digitsHex[lsd];
	} while (i != 0);
	*p = '\0';
	std::reverse(buf,p);
	return p - buf;
}
///
unsigned int murMurHash(const void *key, size_t len,const int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int h = seed ^ len;
	// Mix 4 bytes at a time into the hash
	const unsigned char *data = (const unsigned char *)key;
	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		h *= m; 
		h ^= k;
		data += 4;
		len -= 4;
	}
	// Handle the last few bytes of the input array
	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};
	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h;
}

} //namespace Utility
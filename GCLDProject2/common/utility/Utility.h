
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creator:
// This file add by CaiBingjie 2010-10-29
//
// Porpuse:
// To add some common function ;
//
// TODO: 整合 Util.h
///



#ifndef SHARED_UTILITY_H__
#define SHARED_UTILITY_H__



#include <deque>
#include <string>
#include <cstdio>
#include <cstring>
#include "def/TypeDef.h"
#include "def/MmoAssert.h"
#include "System.h"
#include <boost/static_assert.hpp>
const uint32 MAX_LEN = 255;
using std::string ;


namespace Utility
{
    void FormatString( std::string &sql, const char* szQueryString, ... );
	
	extern const char* zero;
	template<typename T>
	size_t convert(char buf[],T value)
	{
		T i = value;
		char* p = buf;

		do 
		{
			int lsd = static_cast<int>(i % 10);
			i /= 10;
			*p++ = zero[lsd];
		} while ( i != 0);

		if(value < 0)
		{
			*p++ = '-';
		}
		*p='\0';
		std::reverse(buf,p);
		return p -buf;
	}

	size_t converHex(char buf[],uintptr_t value);
	// Clears internal memory of an STL object.
	// STL clear()/reserve(0) does not always free internal memory allocated
	// This function uses swap/destructor to ensure the internal memory is freed.
	template<class T>
	void STLClearObject(T* obj) 
	{
		T tmp;
		tmp.swap(*obj);
		// Sometimes "T tmp" allocates objects with memory (arena implementation?).
		// Hence using additional reserve(0) even if it doesn't always work.
		obj->reserve(0);
	}
	////哈希函数  非一致哈希
	unsigned int murMurHash(const void *key, size_t len,const int seed);

    //////////////////////////////////////////////////////////////////////////
    //  取大数整， 如 取向100 取整， 向10000 取整 , 不满 rounder 则取 rounder
    inline uint32 Rounding( uint32 num, uint32 rounder)
    {
        uint32 result =  num / rounder * rounder;
        if (result < rounder )
        {
            result = rounder;
        }
        return result;
    }

    //////////////////////////////////////////////////////////////////////////
    //  
    template<typename T>
    void LimitValue( T& value , const T& min, const T& max)
    {
        if ( value < min)
        {
            value = min;
        }else if ( value > max)
        {
            value = max;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    void RightShift( std::string& str, unsigned count, char ch = ' ');



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 中概率
    inline bool IsGetPercent( uint32 value, uint32 percent )
    {
	    return value > percent ;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 位操作 以第几位为操作
    // 将第 0 ~ 31 位中的其中一个设置为 1
    inline void SetBit( uint32 &value, uint32 pos )
    {
	    assert( pos < 32 ) ;
	    value |= ( 1 << pos ) ;
    }

    // 将第 0 ~ 31 位中的其中一个设置为 0
    inline void ResetBit( uint32 &value, uint32 pos )
    {
	    assert( pos < 32 ) ;
	    value &= ~( 1 << pos ) ;
    }

    // 返回第 0 ~31 位是否为 1
    inline bool TestBit( uint32 value, uint32 pos )
    {
	    assert( pos < 32 ) ;
	    return ( value & ( 1 << pos ) ) != 0 ;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 位操作 直接以标记为操作
    inline void SetFlag( uint32 &value, uint32 newFlag )
    {
	    value |= newFlag ;
    }

    inline void RemoveFlag( uint32 &value, uint32 oldFlag )
    {
	    value &= ~oldFlag ;
    }

    inline bool HasFlag( uint32 value, uint32 flag )
    {
	    return (value & flag) != 0 ;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 以字符分隔字符串数据读取

    // 在一段以任意符号间隔的字符串里面获取一整型, 且指针移动
    template< typename IntVal >
    bool GetIntValue( char *&pStart, IntVal &value )
    {
	    ASSERT( pStart != NULL ) ;

	    while( *pStart != '\0' && !isdigit( *pStart ) ) {   ++pStart ;  }

	    if( *pStart == '\0' ) {   return false ;   }

	    char *pEnd = pStart ;

	    while( isdigit( *pEnd ) ) {   ++pEnd ;   }

	    bool isNnull = *pEnd == '\0' ;

	    *pEnd ='\0';
	    value = atol(pStart);
	    pStart = isNnull ? pEnd : ++pEnd ;
	    return true ;
    }

	void GetRandStr(string &pStart, uint32 length);

    bool GetOneUInt32Value( char *&pStart, uint32 &value ) ;
    bool GetOneUInt64Value( char *&pStart, uint64 &value ) ;
	///将奖励的物品信息放到string中
	void PacketItemsToString(string& str,const IntPairVec& vec);
	void PacketItemsToString(string& str,const IntPair& pair);
	void PacketItemsToString(string& str,const uint32 type,const uint32 count);

    // 在一段以逗号间隔的字符串里面获取一字符串, 且指针移动
    bool GetOneStringValue( char *&pStart, string &sValue, char delimiter =',' ) ;

	bool GetStringValue(std::vector<int>& retVec,string& outStr,const char split = ',');
	bool GetStringValueForSet( std::set<int>& retSet,string& outStr, const char split = ',' );

    void SplitStr(const string& str, std::vector<string>& retVec, const char split);
    void SplitStr(const string& str, std::vector<int>& retVec, const char split);
	void SplitStr(const string& str, std::set<int>& retSet, const char split);
	void SplitStr(const string& str, std::set<uint64>& retSet, const char split);
	void SplitStr(const string& str,IntPair& p,const char split);//1,2 分割到IntPair中
    // "(2|3)(7|4)(9|6)"，分割为数值对
    void SplitStr(const std::string& str, IntPairVec& retVec);
	// "(2|3,2|3)(7|4,7|4)(9|6,7|8)"，分割为数值对
	void SplitStr(const string& str,VecIntPairvec& retvec);
    // "2|3,7|4,9|6"，分割为数值对
    void SplitStr2(const std::string& str, IntPairVec& retVec);
	// "1|2|3,2|7|4,3|9|6"，分割为数值对
	void SplitStr3(const std::string& str, IntV3Vec& retVec);
	// "1|2|3|4,2|7|4|5,3|9|6|7"，分割为数值对
	void SplitStr4(const std::string& str, IntV4Vec& retVec);

	// "1|2|3,2|7|4,3|9|6"，分割为数值对
	void SplitStrISpair(const std::string& str, ISpairMap& retMap);

	// "2|3,7|4,9|6"，分割为数值对
	void SplitItemStr(const std::string& str, ItemID2Count& retMap);

	// "2_3_7_4_9_6",每两个值为一对
	void SplitAttrLv(const std::string& str, IntPairVec& retVec); 
    // 将 number 位为 1 的单独成为一个值保存到队列中
    void GetPerBitNumberInInteger( const uint32 &number, std::deque<uint32> &deque ) ;
	template <typename NumType>
	void GetValue(const uint64 value,std::vector<NumType>& vec,const uint32 count)
	{
		char str[MAX_LEN] = {0};
		char temp[MAX_LEN] = {0};
		convert(temp,value);
		size_t len = strlen(temp);
		char *pTemp = temp;
		char* pstr = str;
		char* dest = NULL;
		if(IsOdd((int)len) && count != 1)
		{
			pstr[0] = '0';
			pstr++;
			while(*pTemp != '\0')
			{
				*pstr++ = *pTemp++;
			}
			dest = str;
		}
		else
		{
			dest = temp;
		}	
		char *p = dest + strlen(dest);
		while(dest != p)
		{
			vec.push_back(atoi(std::string(p-count,p).c_str()));
			p = p-count;
		}
	}



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template< class InserterAdapter >
    void GetStringList(char *pCur, InserterAdapter stringList, char delimiter =',' )
    {
	    string tempValue ;
	    while( GetOneStringValue( pCur, tempValue, delimiter ) )
	    {
		    stringList =tempValue ;
	    }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 在分隔符序列中获取一个数字序列 ( 1,2,3,4,5,6,7,8,9 ), InserterAdapter 请使用插入适配器，本函数以 = 赋值
    template< class InserterAdapter >
    void GetNumericalList( string &strContent, InserterAdapter numericalList, bool ignoreZero =false )
    {
	    //char *pBeg =&strContent[0] ;
	    char *pBeg = const_cast<char*>( strContent.c_str() ) ;
    	
	    typename InserterAdapter::container_type::value_type value =0;

	    while( GetIntValue( pBeg, value ) )
	    {
		    if( ignoreZero && value == 0 )
		    {
			    continue ;
		    }
		    numericalList =value ;
	    }
    }

    template< typename T1, typename T2>
    void AddUp( std::map<T1, T2>& to, const std::map<T1, T2>& from)
    {
        for ( typename std::map<T1, T2>::const_iterator it = from.begin(); it != from.end(); ++it)
        {
            to[ it->first] += it->second;
        }
    }


    template< typename T1, typename T2, typename RoundPolicy>
    void MultiplyValue( std::map<T1, T2>& Key_Val, float factor, RoundPolicy policy)
    {
        for ( typename std::map<T1, T2>::iterator it = Key_Val.begin(); it != Key_Val.end(); ++it)
        {
            it->second = T2( policy(it->second * factor));
        }
    }

}  //namespace Utility


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 原子操作， 使用 C++11 時去掉
//#if PLATFORM == PLATFORM_WIN32
//#define AutomicCompareExchange( pDst, v0, v1 ) InterlockedCompareExchange( (pDst), (v1), (v0) )
//#else if PLATFORM == PLATFORM_UNIX
//#define AutomicCompareExchange( pDst, v0, v1 ) __sync_val_compare_and_swap( (pDst), (v0), (v1) )
//#endif
//
//#if PLATFORM == PLATFORM_WIN32
//#define AutomicIncrement( pDst ) InterlockedIncrement( (pDst) )
//#else if PLATFORM == PLATFORM_UNIX
//#define AutomicIncrement( pDst ) __sync_add_and_fetch( (pDst), 1 )
//#endif


#endif

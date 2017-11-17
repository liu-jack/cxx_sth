#include "TimeParser.h"
#include <assert.h>
#include <memory.h>
#include "../OS.h"



//用一个本地时间戳，转化成一个GMT的时间字符串
bool TimeT2String( const time_t timeToConvert, std::string& outStr, bool isGMT /*= false*/)
{
	if (timeToConvert < 0)
	{
		return false ;
	}
	char szTime[20] = {0};

    if ( isGMT)
    {
        strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", gmtime((const time_t*)&timeToConvert));  
    }
    else
    {
		struct tm tm_info;
		sOS.LocalTime(tm_info,timeToConvert);
        strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", &tm_info);
    }
    

	outStr = szTime;
	return true;
}
//转换为GMT时间戳
///@stringToConvert 是一个GMT时间  '2017-9-1 12:00:00'  @outTimeT是一个本地时间戳
bool String2TimeT( const std::string &stringToConvert, time_t& outTimeT, bool isGMT /*= false*/)
{

	bool bHasMinus =stringToConvert.find( '-' ) != std::string::npos ;
	bool bHasColon =stringToConvert.find( ':' ) != std::string::npos ;

	tm tm1;
	::memset( &tm1, 0, sizeof( tm ) ) ;

	if( bHasMinus && bHasColon )
	{
		sscanf(stringToConvert.c_str(),"%d-%d-%d %d:%d:%d", &(tm1.tm_year), &(tm1.tm_mon), &(tm1.tm_mday), &(tm1.tm_hour), &(tm1.tm_min), &(tm1.tm_sec) ) ;
		tm1.tm_year -= 1900;
		tm1.tm_mon --;
	}
	else if( bHasMinus )
	{
		sscanf(stringToConvert.c_str(),"%d-%d-%d" , &(tm1.tm_year), &(tm1.tm_mon), &(tm1.tm_mday) ) ;

		tm1.tm_year -= 1900;
		tm1.tm_mon --;
	}
	else if( bHasColon )
	{
		tm1.tm_year =80 ;
		tm1.tm_mon = 0;
		tm1.tm_mday =1;
		sscanf(stringToConvert.c_str(),"%d:%d:%d" , &(tm1.tm_hour), &(tm1.tm_min), &(tm1.tm_sec) ) ;
		// 只有时间的直接计算返回，因为mktime()年份要从1970开始，不然返回-1，失败！
		//outTimeT = tm1.tm_hour * 60 * 60 + tm1.tm_min * 60 + tm1.tm_sec ;

		//int h =0, m=0, s=0 ;
		//TimeT2Time( outTimeT, h, m, s ) ;
		//assert( h == tm1.tm_hour ) ;
		//assert( m == tm1.tm_min ) ;
		//assert( s == tm1.tm_sec ) ;
	}

	tm1.tm_isdst=-1;

	outTimeT = 0;
	outTimeT = ::mktime(&tm1);
	if (outTimeT == -1)
	{
        outTimeT = 0;
		return false;
	}
    if (isGMT)
    {
        outTimeT += get_gmt_offset();
    }
    

	//int y=0, m=0, d=0 ;
	//TimeT2Date( outTimeT, y, m, d ) ;
	//assert( y - 1900 == tm1.tm_year ) ;
	//assert( m - 1    == tm1.tm_mon ) ;
	//assert( d    == tm1.tm_mday ) ;

	return true;
}

bool TimeT2Date(const time_t inDate, int& outYear, int& outMonth, int& outDay)
{
	struct tm tm_info;
	sOS.LocalTime(tm_info,inDate);
	outYear  =tm_info.tm_year + 1900 ;
	outMonth =tm_info.tm_mon + 1 ;
	outDay   =tm_info.tm_mday ;
	return true;

}

bool TimeT2Time(const time_t inTime, int& outH, int& outM, int& outS)
{
	time_t useTime =inTime;
	if (inTime == 0)
	{
		tm tempTm ;
		::memset( &tempTm, 0, sizeof( tempTm ) ) ;
		tempTm.tm_year =80 ;
		tempTm.tm_mon = 0;
		tempTm.tm_mday =1;
		tempTm.tm_hour = 0;
		tempTm.tm_min = 0;
		tempTm.tm_sec = 0;
		tempTm.tm_isdst=-1;
		time_t time1 = 0;
		time1 = ::mktime(&tempTm);
		if (time1 == -1)
		{
			return false;
		}
		useTime = time1;
	}
	struct tm timestruct;
	sOS.LocalTime(timestruct,useTime);
	outH =timestruct.tm_hour ;
	outM =timestruct.tm_min ;
	outS =timestruct.tm_sec ;
	return true;
}

time_t JustGetTime( const time_t inTime )
{
	int h =0, m =0, s =0 ;
	TimeT2Time( inTime, h, m, s ) ;

	return h * 60 * 60 + m * 60 + s ;
}

namespace{
    double inner_get_gmt_offset()
    {
        time_t now = time(NULL);

        struct tm *gm = gmtime(&now);
        time_t gmt = mktime(gm);

        return difftime(now, gmt);
    }
}


time_t get_gmt_offset()
{
    static time_t off_set = (time_t)inner_get_gmt_offset();
    return off_set;
}

#ifndef _TIME_PARSER_H_
#define _TIME_PARSER_H_

#include <time.h>
#include <string>

bool TimeT2String(const time_t timeToConvert,std::string& outStr, bool isGMT = false);
bool String2TimeT(const std::string &stringToConvert, time_t& outTimeT, bool isGMT = false);

bool TimeT2Date(const time_t inDate, int& outYear, int& outMonth, int& outDay);
bool TimeT2Time(const time_t inTime, int& outH, int& outM, int& outS);

time_t JustGetTime( const time_t inTime ) ;

time_t get_gmt_offset();


#endif

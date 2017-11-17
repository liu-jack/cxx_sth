#ifndef TimePeriod_h__
#define TimePeriod_h__


#include "SimpleSingleton.h"
#include "TimeParser.h"
#include "OS.h"

#define SECONDS_PER_DAY ( 60*60*24 )
#define SECONDS_PER_WEEK ( SECONDS_PER_DAY*7 )
#define MONTHS_PER_YEAR 12

typedef int Second;
static const int TIME_PERIOD_MONTHLY = 0; 


template<class Tag>
class TimePeriod : public SimpleSingleton<TimePeriod<Tag>>
{
    friend class SimpleSingleton< TimePeriod<Tag>>;
public:
    void InitBeginTime( const std::string& beginTimeStr);
    time_t GetBeginTime() const { return period_begin_time; }
    time_t GetTimePeriod(time_t t,  Second second) const;
	time_t GetNowTimePeriod(Second second) const;
    bool IsInSameTimePeriod( time_t t1, time_t t2, Second second) const;
    bool IsInTodayTimePeriod(time_t testT,Second second);
    time_t GetNowPeriodBeginTime( Second second) const;
    time_t GetNowPeriodEndTime( Second second) const;

    time_t GetPeriodBeginTime( time_t period, time_t t, Second second) const;
private:
    TimePeriod(){}
    ~TimePeriod(){}
    time_t period_begin_time;
};

template<class Tag>
bool TimePeriod<Tag>::IsInTodayTimePeriod( time_t testT,Second second )
{
	time_t nowPeriod = GetTimePeriod( sOS.GetRealTime(), second);
	time_t testPeriod = GetTimePeriod( testT, second);
	if (testPeriod >= nowPeriod)
	{
		return true;
	}
	return false;
}

template<class Tag>
time_t TimePeriod<Tag>::GetNowTimePeriod( Second second ) const
{
	return GetTimePeriod( sOS.GetRealTime(), second);
}

template<class Tag>
time_t TimePeriod<Tag>::GetPeriodBeginTime( time_t period , time_t t,Second second) const
{
    switch(second)
    {
    case TIME_PERIOD_MONTHLY:
        {
			struct tm timestruct;
			sOS.LocalTime(timestruct,t);
            int year = timestruct.tm_year;
            int month = timestruct.tm_mon;
            memset(&timestruct,0,sizeof(tm));
            timestruct.tm_year = year;
            timestruct.tm_mon = month;
            timestruct.tm_mday = 1;            
            return ::mktime(&timestruct);
        }
    default:
        return period_begin_time + period * second;
    }
}

template<class Tag>
time_t TimePeriod<Tag>::GetNowPeriodEndTime( Second second ) const
{
    time_t now = sOS.GetRealTime();
    time_t period = GetTimePeriod( now, second) + 1;
    return GetPeriodBeginTime(period, now, second);
}

template<class Tag>
time_t TimePeriod<Tag>::GetNowPeriodBeginTime( Second second ) const
{
    time_t now = sOS.GetRealTime();
    time_t period = GetTimePeriod(now, second);
    return GetPeriodBeginTime(period, now, second);
}



template<class Tag>
void TimePeriod<Tag>::InitBeginTime(const std::string& beginTimeStr)
{
    String2TimeT( beginTimeStr, period_begin_time , false);
	//NLOG("period_begin_time=%lld",period_begin_time);
}

template<class Tag>
time_t TimePeriod<Tag>::GetTimePeriod(time_t t,  Second second) const
{
    assert(period_begin_time != 0);
    switch(second)
    {
    case TIME_PERIOD_MONTHLY:
        {
			struct tm timestruct;
			sOS.LocalTime(timestruct,t);
            return (timestruct.tm_year - 100 ) * MONTHS_PER_YEAR + timestruct.tm_mon; // month since 2000 ,100 because tm_year since 1900 
        }
    default:
        return static_cast<time_t>( (t - period_begin_time ) / second );
    }
}

template<class Tag>
bool TimePeriod<Tag>::IsInSameTimePeriod(time_t t1, time_t t2, Second second) const
{
    return GetTimePeriod(t1, second) == GetTimePeriod(t2, second);
}

class TagSystem {};
class TagEvent{};
class TagSettle{};
class TagSimpleUnique{};
class TagSimpleTwentyThree{};

#define sPeriodSystem  TimePeriod<TagSystem>::Instance()         // 公共系统结算时间2012-1-1 04:00:00
#define sPeriodEvent       TimePeriod<TagEvent>::Instance()      // 公共活动重置时间2012-1-1 00:00:00
#define sPeriodSettle       TimePeriod<TagSettle>::Instance()    // 公共结算时间2012-1-1 22:00:00
#define sPeriodSimple       TimePeriod<TagSimpleUnique>::Instance()    // 少数系统重置时间2012-1-1 23:10:00
#define sPeriodSimpleTwentyThree       TimePeriod<TagSimpleTwentyThree>::Instance()    // 少数系统重置时间2012-1-1 23:00:00

#endif // TimePeriod_h__

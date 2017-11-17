#ifndef _OS_H_
#define _OS_H_

#include <time.h>
#include <sys/timeb.h>


#include "def/TypeDef.h"
#include "SimpleSingleton.h"

#define ONE_DAY_SECOND (24*3600)
#define ONE_HOUR_SECOND (3600)
#define ONE_MONTH_SECOND (ONE_DAY_SECOND * 30)

#ifdef WIN32
	typedef struct _timeb TimeB ;
#else
	typedef struct timeb TimeB ;
#endif


class OS:public SimpleSingleton<OS>
{
private:
	friend class SimpleSingleton<OS>;

private:
	OS() {}
	static time_t	s_realTime;
    static time_t	s_cheatTime;
public:
	virtual ~OS() {}

	void SleepS(unsigned long sec);
	void SleepMS(unsigned long msec);

	static uint64 TimeMS( void ) ;
	static uint64 TimeSeconds( void ) ;
    static int TimeHour();
    static int TimeMonth();
    static int TimeDayOfWeek();
    //static struct tm* LocalTime();
	void LocalTime(struct tm& outTime, const time_t& inTime);
	void LocalTime(struct tm& outTime);
	static uint64 DiffTimeMS( uint64 forTime);

    bool IsToday(time_t sec);
    bool IsSameDay(time_t sec1, time_t sec2);

	int64 ParseTime(int64 num); //20160223145632£º2016Äê2ÔÂ23ºÅ14:56:32

	std::string GetModulePath();

	TimeB GetTimeB( void ) ;

	unsigned long GetCurrentThreadID();

	//random number generator - really fast
	static void SRand();
	static void SRand(int seed);
	static int Rand();
	static int Rand(int lowerBound, int upperBound);
	static float Randf();
	static float Randf(float lowerBound, float upperBound);

	static time_t GetRealTime();
	static void Update();
    static void AddCheatTime( time_t cheatTime);
	static time_t getGmTime();
};


#define sOS (OS::Instance())

#endif


#include "OS.h"
#include "Logger.h"

#include "Base.h" // 要放在 windwos.h 前面

// TimeMs() Methon use
#include <sys/timeb.h>
#include <time.h>
#include "def/MmoAssert.h"


#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <sys/syscall.h>
#endif

time_t  OS::s_cheatTime = 0;

void OS::SleepS(unsigned long sec)
{
#ifdef WIN32
	Sleep(sec * 1000);
#else
	sleep(sec);
#endif
}


void OS::SleepMS(unsigned long msec)
{
#ifdef WIN32
	Sleep(msec);
#else
	usleep(msec * 1000);
#endif
}


uint64 OS::TimeMS( void )
{
#ifdef WIN32
	struct _timeb tp ;
	::_ftime( &tp ) ;
	return tp.time * 1000 + tp.millitm;
#else
	struct timeb tp;
	ftime(&tp);
	return tp.time * 1000 + tp.millitm;
#endif
}

uint64 OS::TimeSeconds( void )
{
	return TimeMS()/1000 ;
}
int OS::TimeHour()
{
	time_t timep;
	time(&timep);
	struct tm p;
#ifdef WIN32
	localtime_s(&p,&timep);
#else
	localtime_r(&timep,&p);
#endif
    return p.tm_hour;
}
int OS::TimeMonth()
{
	time_t timep;
	time(&timep);
	struct tm p;
#ifdef WIN32
	localtime_s(&p,&timep);
#else
	localtime_r(&timep,&p);
#endif
    return p.tm_mon;
}
int OS::TimeDayOfWeek()
{
    time_t timep;
    time(&timep);
	struct tm p;
#ifdef WIN32
	localtime_s(&p,&timep);
#else
	localtime_r(&timep,&p);
#endif
    return p.tm_wday;
}
//struct tm* OS::LocalTime()
//{
//    time_t timep;
//    time(&timep);
//    return localtime(&timep);
//}

void OS::LocalTime(struct tm& outTime)
{
#ifdef WIN32
	struct _timeb tp ;
	::_ftime( &tp ) ;
	localtime_s(&outTime,&tp.time);
#else
	struct timeb tp;
	ftime(&tp);
	localtime_r(&tp.time,&outTime);
#endif
}

void OS::LocalTime(struct tm& outTime, const time_t& inTime)
{
	time_t useTime =inTime;
#ifdef WIN32
	localtime_s(&outTime,&useTime);
#else
	localtime_r(&useTime,&outTime);
#endif
}

uint64 OS::DiffTimeMS(uint64 forTime)
{
	uint64 curTime = TimeMS();
	return curTime > forTime ?  (curTime - forTime) : 0;
}

TimeB OS::GetTimeB( void )
{
	TimeB b ;
#ifdef WIN32
	::_ftime( &b ) ;
#else
	::ftime(&b);
#endif
	return b ;
}
static time_t _GetCurDay(time_t sec){
	struct tm t;
	sOS.LocalTime(t,sec);
    time_t ret = t.tm_year;
    ret = ret << 8;
    ret += t.tm_mon;
    ret = ret << 8;
    ret += t.tm_mday;
    return ret;
}
bool OS::IsToday(time_t sec)
{
    return _GetCurDay(sec) == _GetCurDay(::time(NULL));
}
bool OS::IsSameDay(time_t sec1, time_t sec2)
{
    return _GetCurDay(sec1) == _GetCurDay(sec2);
}
int64 OS::ParseTime(int64 num) //20160223145632：2016年2月23号14:56:32
{
#define _Parse_(n) num%(n); num /= (n)
	struct tm t;
	t.tm_sec = _Parse_(100);
	t.tm_min = _Parse_(100);
	t.tm_hour = _Parse_(100);
	t.tm_mday = _Parse_(100);
	t.tm_mon = _Parse_(100);
	t.tm_year = _Parse_(10000);
	t.tm_year -= 1900;
	t.tm_isdst = 0;
#undef _Parse_
	return mktime(&t);
}

std::string OS::GetModulePath()
{
	static string modulePath;
	if( modulePath.empty() )
	{
#ifdef WIN32
		char tmpBuf[MAX_PATH+1] = {0};
		GetModuleFileNameA( NULL, tmpBuf, MAX_PATH );
		*(strrchr( tmpBuf, '\\' ) + 1) = '\0';
		modulePath = tmpBuf;
#else
		char tmpBuf[1024] = {0};
		ssize_t ret =readlink("/proc/self/exe", tmpBuf, sizeof(tmpBuf) - 1);
		if( ret == -1 )
		{
			tmpBuf[0] ='/' ;
		}

		*(strrchr( tmpBuf, '/' ) + 1) = '\0';
		modulePath = tmpBuf;
#endif
	}
	return modulePath;
}


unsigned long OS::GetCurrentThreadID()
{
#if defined(WIN32) || defined(WIN64)
	return GetCurrentThreadId();
#else
	return syscall(__NR_gettid);
#endif
}



void OS::SRand()
{
	SRand((int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////

void OS::SRand(int seed)
{
	srand( seed );
	/*m_rndx = seed;
	m_rndy = 842502087L;
	m_rndz = 3579807591L;
	m_rndw = 273326509L;*/
}

//////////////////////////////////////////////////////////////////////////

int OS::Rand()
{
	return rand();
	/*long long t	=(m_rndx^(m_rndx<<11));
	m_rndx	= m_rndy; m_rndy = m_rndz; m_rndz = m_rndw;
	m_rndw	=(m_rndw^(m_rndw>>19))^(t^(t>>8));

	long long rtn = m_rndw&0x7FFFFFFF;
	if (rtn == 0x7FFFFFFF)
	return Rand();

	return (int)rtn;			*/
}

//////////////////////////////////////////////////////////////////////////
//   return value range [lowerBound, upperBound)

int OS::Rand(int lowerBound, int upperBound)
{
	if (lowerBound == upperBound || lowerBound +1 == upperBound)
		return lowerBound;
	if (lowerBound > upperBound)
	{
		int t		= lowerBound;
		lowerBound	= upperBound;
		upperBound	= t;
	}

	long long rnd	= Rand();
	int	 range		= upperBound - lowerBound;

	//return (int)(lowerBound + (rnd * range >> 31)); //buggy for lower values
	int res = (int)(lowerBound + (rnd % range));
	ASSERT(res >= lowerBound && res < upperBound);

	return res;
}

//////////////////////////////////////////////////////////////////////////

float OS::Randf()
{
	int rand = Rand();
	rand &= 0xFFFF;
	rand -= 0x7FFF;

	float randf = ((float)rand) * (1.0f / 32768.5f);
	ASSERT(randf > -1.f && randf < 1.f);

	return randf;
}

//////////////////////////////////////////////////////////////////////////

float OS::Randf(float lowerBound, float upperBound)
{
	if (lowerBound > upperBound)
	{
		float t		= lowerBound;
		lowerBound	= upperBound;
		upperBound	= t;
	}

	float diff = upperBound - lowerBound;
	float rnd = Randf();
	return lowerBound + (ABS(rnd)) * diff;
}

void OS::Update()
{
}

time_t OS::GetRealTime()
{
    return TimeSeconds() + s_cheatTime;
}

void OS::AddCheatTime( time_t cheatTime )
{
    s_cheatTime += cheatTime;
}

time_t OS::getGmTime()
{
	time_t now = time(NULL);

	struct tm *gm = gmtime(&now);
	time_t gmt = mktime(gm);
	return gmt;
}
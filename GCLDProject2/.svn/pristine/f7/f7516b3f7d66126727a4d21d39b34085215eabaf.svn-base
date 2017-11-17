#include "preHeader.h"
#include "System.h"
#include "def/MmoAssert.h"
#include "Base.h"

//static long long m_rndx, m_rndy, m_rndz, m_rndw;
long long System::s_appStartTime;
time_t	System::s_realTime;
//tm		System::s_realTM;

//////////////////////////////////////////////////////////////////////////

void System::SRand()
{
	srand((int)time(NULL));
	//	SRand((int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////

void System::SRand(int seed)
{
	srand( seed );
	/*m_rndx = seed;
	m_rndy = 842502087L;
	m_rndz = 3579807591L;
	m_rndw = 273326509L;*/
}

//////////////////////////////////////////////////////////////////////////

int System::Rand()
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

int System::Rand(int lowerBound, int upperBound)
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

float System::Randf()
{
	int rand = Rand();
	rand &= 0xFFFF;
	rand -= 0x7FFF;

	float randf = ((float)rand) * (1.0f / 32768.5f);
	ASSERT(randf > -1.f && randf < 1.f);

	return randf;
}

//////////////////////////////////////////////////////////////////////////

float System::Randf(float lowerBound, float upperBound)
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

void System::Update()
{
	s_realTime = time(NULL);
}

//tm System::GetRealDateTime()
//{
//	return s_realTM;
//}

time_t System::GetRealTime()
{
	return s_realTime;
}

long System::CurrentTimeMillis()
{	
#ifdef WIN32
	return GetTickCount();
#else
	struct timeb tp;
	ftime(&tp);
	return tp.time * 1000 + tp.millitm;
#endif
}

unsigned int System::GetAppTime()
{
	return (unsigned int)(CurrentTimeMillis() - s_appStartTime);
}

void System::GetMemState(MEM_STATE& mem)
{
	return;
}

int System::CalcCpuOccupy (CPU_STATE& o, CPU_STATE& n)
{  
	return 0;
}

void System::GetCPUState(CPU_STATE& cpust)
{  
	return;
}

int System::GetAppInfo(AppInfo * pinfo)
{
	return 0;
}

#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)
int  System::GetThreadsStat(ThreadInfo infos[], int size)
{
	return 0;
}

bool System::IsDigit(const char* s)
{
    const char* tmp = s;
    for(; *tmp != '\0'; ++tmp)
    {
        if(!isdigit(*tmp)) break;
    }
    return *tmp == '\0';
}

void  System::GetSubCpusStat(CPU_STATE stats[], int size)
{
	return;
}

unsigned long long System::GetProcessUseTimeMs()
{
	return 0;
}

unsigned long long System::TimeMs2Tick(unsigned long long timeMs)
{
	return 0;
}

unsigned long long System::GetCpuTimeMs()
{
	return 0;
}

int System::GetCpuNum()
{
	return 0;
}
#endif // _MMO_SERVER_

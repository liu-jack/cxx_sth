#include "AutoUpdateCross.h"
#include "OS.h"
#include "common/utility/TimePeriod.h"
#include "Logger.h"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "def/TypeDef.h"
#include "boost/bind/bind.hpp"

AutoUpdateCross::AutoUpdateCross()
:m_eventTimer()
{

}

void AutoUpdateCross::InitAddFunctions()
{

}

void AutoUpdateCross::Update(uint64 nowTimeMS,int64 diffMS)
{
	m_eventTimer.Update(diffMS);
}


void AutoUpdateCross::AddEvent( boost::function<void()> f, U32 start, int repeat /*= 1*/, U32 interval /*=0√Î*/ )
{
	m_eventTimer.AddEvent(f,start,repeat,interval);
}

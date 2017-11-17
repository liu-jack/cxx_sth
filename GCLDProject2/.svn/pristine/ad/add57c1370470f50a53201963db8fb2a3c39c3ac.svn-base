#include "OS.h"
#include "AutoUpdateBase.h"
#include "common/utility/TimePeriod.h"


void AutoUpdateBase::Init()
{
	m_periodMap[auto_update_enum::PER_SECOND]		= SECOND_LONG;
	m_periodMap[auto_update_enum::PER_HALF_MINUTE]	= HALF_MINUTE_LONG;
	m_periodMap[auto_update_enum::PER_MINUTE]		= MINUTE_LONG;
	m_periodMap[auto_update_enum::PER_HALF_HOUR]		= HALF_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_HOUR]			= HOUR_LONG;
	m_periodMap[auto_update_enum::PER_TWO_HOUR]		= TWO_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_THREE_HOUR]	= THREE_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_FOUR_HOUR]		= FOUR_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_SIX_HOUR]		= SIX_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_EIGHT_HOUR]	= EIGHT_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_TWELVE_HOUR]	= TWELVE_HOUR_LONG;
	m_periodMap[auto_update_enum::PER_DAY]			= DAY_LONG;
	m_periodMap[auto_update_enum::PER_WEEK]			= WEEK_LONG;
	m_periodMap[auto_update_enum::PER_MONTH]			= MONTH_LONG;

	InitAddFunctions();
}

void AutoUpdateBase::Add(boost::function<void()> func, auto_update_enum::timeCheckPoint checkPoint, auto_update_enum::timeCheckPeriod checkPeriod)
{
	//NLOG("AutomaticUpdate::Add");
	boost::shared_ptr<UpdateEventBase> upe(new UpdateEventBase(func,checkPeriod,checkPoint));
	upe->SetLastUpdateTime(sOS.TimeSeconds());
	m_updateMap.push_back(upe);
}


auto_update_enum::timeCheckPeriod AutoUpdateBase::GetTimePeriod(uint64 time)
{
	PeriodMap::iterator it = m_periodMap.begin(); 
	for(; it != m_periodMap.end(); it++)
	{
		if(it->second == time)
			return it->first;
	}
	return auto_update_enum::TIME_NULL;
}
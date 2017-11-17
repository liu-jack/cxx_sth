#include "AutomaticUpdate.h"
#include "OS.h"
#include "common/utility/TimePeriod.h"
#include "../session/PlayerPool.h"
#include "Map/WorldMap.h"
#include "Map/country/country.h"
#include "Palace/PalaceMgr.h"
#include "Exploit/ExploitMgr.h"
#include "Map/PlayerMapLogic.h"

//static const AutomaticUpdate::UpdateFunc g_handler[] = 
//{
//
//}

void AutomaticUpdate::Init()
{
	m_periodMap[detail_enum::PER_SECOND]		= SECOND_LONG;
	m_periodMap[detail_enum::PER_HALF_MINUTE]	= HALF_MINUTE_LONG;
	m_periodMap[detail_enum::PER_MINUTE]		= MINUTE_LONG;
	m_periodMap[detail_enum::PER_HALF_HOUR]		= HALF_HOUR_LONG;
	m_periodMap[detail_enum::PER_HOUR]			= HOUR_LONG;
	m_periodMap[detail_enum::PER_TWO_HOUR]		= TWO_HOUR_LONG;
	m_periodMap[detail_enum::PER_THREE_HOUR]	= THREE_HOUR_LONG;
	m_periodMap[detail_enum::PER_FOUR_HOUR]		= FOUR_HOUR_LONG;
	m_periodMap[detail_enum::PER_SIX_HOUR]		= SIX_HOUR_LONG;
	m_periodMap[detail_enum::PER_EIGHT_HOUR]	= EIGHT_HOUR_LONG;
	m_periodMap[detail_enum::PER_TWELVE_HOUR]	= TWELVE_HOUR_LONG;
	m_periodMap[detail_enum::PER_DAY]			= DAY_LONG;
	m_periodMap[detail_enum::PER_WEEK]			= WEEK_LONG;
	m_periodMap[detail_enum::PER_MONTH]			= MONTH_LONG;

	Add(boost::bind(&AutomaticUpdate::DoUpdatePer_Day_Zero,this),detail_enum::TIME_ZERO,detail_enum::PER_DAY);
	Add(boost::bind(&AutomaticUpdate::DoUpdatePer_Day_TwentyThreeAndTen,this),detail_enum::TIME_TWENTYTHREE_AND_TEN,detail_enum::PER_DAY);

	Add(boost::bind(&AutomaticUpdate::DoUpdatePer_FourHour_Zero,this),detail_enum::TIME_ZERO,detail_enum::PER_FOUR_HOUR);
	Add(boost::bind(&AutomaticUpdate::DoUpdatePer_Hour_Zero,this),detail_enum::TIME_ZERO,detail_enum::PER_HOUR);
	Add(boost::bind(&AutomaticUpdate::DoUpdatePer_Second_Zero,this),detail_enum::TIME_ZERO,detail_enum::PER_SECOND);
	
	//Add(boost::bind(&ExploitMgr::InitializeExploitValue,this),detail_enum::TIME_TWENTYTHREE_AND_TEN,detail_enum::PER_DAY);
}

void AutomaticUpdate::Add(boost::function<void()> func, detail_enum::timeCheckPoint checkPoint, detail_enum::timeCheckPeriod checkPeriod)
{
	//NLOG("AutomaticUpdate::Add");
	boost::shared_ptr<UpdateEvent> upe(new UpdateEvent(func,checkPeriod,checkPoint));
	m_updateMap.push_back(upe);
}



void AutomaticUpdate::Update()
{
	time_t nowtime = sOS.TimeSeconds();
	UpdateMap::iterator it = m_updateMap.begin();
	struct tm* local_time = NULL;

	bool retbool = true;
	for(;it != m_updateMap.end(); )
	{
		retbool = true;
		switch ((*it)->GetCheckPoint())
		{
		case detail_enum::TIME_ZERO:
			if(!doUpdateZero(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		case detail_enum::TIME_FOUR:
			if(!doUpdateFour(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		case detail_enum::TIME_TWENTYTWO:
			if(!doUpdateTwentyTwo(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		case detail_enum::TIME_TWENTYTHREE_AND_TEN://每天
			if(!doUpdateTwentyThreeAndTen(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		default:
			break;
		}

		if(retbool)
			it++;
	}
}

bool AutomaticUpdate::doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEvent> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodEvent.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))		//sPeriodEvent基准时间为0点，所以每0点开始，经过一个timelong为一个时间片，看是否在同一个时间片中
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

bool AutomaticUpdate::doUpdateFour(time_t nowtime, boost::shared_ptr<UpdateEvent> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSystem.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

bool AutomaticUpdate::doUpdateTwentyTwo(time_t nowtime, boost::shared_ptr<UpdateEvent> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSettle.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}


bool AutomaticUpdate::doUpdateTwentyThreeAndTen(time_t nowtime, boost::shared_ptr<UpdateEvent> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSimple.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

detail_enum::timeCheckPeriod AutomaticUpdate::GetTimePeriod(uint64 time)
{
	PeriodMap::iterator it = m_periodMap.begin(); 
	for(; it != m_periodMap.end(); it++)
	{
		if(it->second == time)
			return it->first;
	}
	return detail_enum::TIME_NULL;
}


void AutomaticUpdate::DoUpdatePer_Day_Zero()
{
   sPlayerPool.DoUpdatePer_Day_Zero();		//对每个玩家执行0基准重置操作

   for(int i=1; i<=3; i++)
   {
	   sWorldMap.GetCountry(i)->updateNpcExpPerDay();
   }
}

void AutomaticUpdate::DoUpdatePer_FourHour_Zero()
{
	sPlayerPool.DoUpdatePer_FourHour_Zero();		//对每个玩家执行0基准4小时刷新操作
}

void AutomaticUpdate::DoUpdatePer_Day_TwentyThreeAndTen()
{
	sExploitMgr.InitializeExploitValue();
}

void AutomaticUpdate::DoUpdatePer_Hour_Zero()
{
	sPlayerPool.DoUpdatePer_Hour_Zero();

	for(int i=1; i<=3; i++)
	{
		sPalaceMgr(i)->UpdateOfficalAwardsPerHour();
	}
}

void AutomaticUpdate::DoUpdatePer_Second_Zero()
{
	sPlayerPool.DoUpdatePer_Second_Zero();		//消耗太大，避免使用这个

	PlayerMapLogic::UpdateCityEvent();
}
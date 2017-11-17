#include "AutoUpdateLobby.h"
#include "OS.h"
#include "common/utility/TimePeriod.h"
#include "Logger.h"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "def/TypeDef.h"
#include "boost/bind/bind.hpp"
#include "logic/country/war/CrossWarDestServerMgr.h"

void AutoUpdateLobby::InitAddFunctions()
{
	Add(boost::bind(&AutoUpdateLobby::DoUpdatePer_Day_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_DAY);
	//Add(boost::bind(&AutoUpdateLobby::DoUpdatePer_Hour_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_HOUR);
}

void AutoUpdateLobby::Update(uint64 nowTimeMS,int64 diffMS)
{
	time_t nowtime = nowTimeMS/1000;
	UpdateMap::iterator it = m_updateMap.begin();
	struct tm* local_time = NULL;

	bool retbool = true;
	for(;it != m_updateMap.end(); )
	{
		retbool = true;
		switch ((*it)->GetCheckPoint())
		{
		case auto_update_enum::TIME_ZERO:
			if(!doUpdateZero(nowtime,(*it)))
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

bool AutoUpdateLobby::doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodEvent.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))		//sPeriodEvent基准时间为0点，所以每0点开始，经过一个timelong为一个时间片，看是否在同一个时间片中
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

void AutoUpdateLobby::DoUpdatePer_Day_Zero()
{
	//跨服灭国是否开启
	int timenow = sOS.TimeDayOfWeek();
	if(timenow == auto_update_enum::Tuesday || timenow == auto_update_enum::Thursday || timenow == auto_update_enum::Saturday)
	{
		LLOG("[1002] Start Cross Server War");
		sCrossWarDestSerMgr.StartCrossWarDest();
	}
}

void AutoUpdateLobby::DoUpdatePer_Hour_Zero()
{
	//跨服灭国是否开启
	//int timenow = sOS.TimeDayOfWeek();
	//if(timenow == auto_update_enum::Tuesday || timenow == auto_update_enum::Thursday || timenow == auto_update_enum::Saturday)
	//{
		LLOG("[1002] Start Cross Server War");
		sCrossWarDestSerMgr.StartCrossWarDest();
	//}
}

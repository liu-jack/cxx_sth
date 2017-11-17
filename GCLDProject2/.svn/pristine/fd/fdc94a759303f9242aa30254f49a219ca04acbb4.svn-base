#include "AutoUpdateGame.h"
#include "OS.h"
#include "common/utility/TimePeriod.h"
#include "Logger.h"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "boost/bind/bind.hpp"

#include "../session/PlayerPool.h"
#include "Map/WorldMap.h"
#include "Map/country/country.h"
#include "Palace/PalaceMgr.h"
#include "Exploit/ExploitMgr.h"
#include "Map/PlayerMapLogic.h"
#include "ActivityControl/ActivityMgr.h"
#include "CrossLogic/PlayerActivityStarBoxDB.h"
#include "CrossLogic/PlayerGeneralRewardDB.h"
#include "CrossLogic/ActivityOccupyCityDB.h"
#include "ActivityRank/table/ActivityRankTableMgr.h"
extern void GetRankFromValue(std::vector<int>& vec);
extern uint32 activity_occupy_city_god;
extern uint32 activity_occupy_city_human;
extern uint32 activity_occupy_city_worm;
AutoUpdateGame::AutoUpdateGame()
:m_eventTimer()
{

}

void AutoUpdateGame::InitAddFunctions()
{
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_Day_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_DAY);
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_Day_TwentyThreeAndTen,this),auto_update_enum::TIME_TWENTYTHREE_AND_TEN,auto_update_enum::PER_DAY);
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_Day_TwentyThree,this),auto_update_enum::TIME_TWENTYTHREE,auto_update_enum::PER_DAY);
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_FourHour_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_FOUR_HOUR);
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_Hour_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_HOUR);
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_Second_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_SECOND);
	Add(boost::bind(&AutoUpdateGame::DoUpdatePer_Minute_Zero,this),auto_update_enum::TIME_ZERO,auto_update_enum::PER_MINUTE);
}

void AutoUpdateGame::Update(uint64 nowTimeMS,int64 diffMS)
{
	m_eventTimer.Update(diffMS);

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
		case auto_update_enum::TIME_FOUR:
			if(!doUpdateFour(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		case auto_update_enum::TIME_TWENTYTWO:
			if(!doUpdateTwentyTwo(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		case auto_update_enum::TIME_TWENTYTHREE_AND_TEN://每天
			if(!doUpdateTwentyThreeAndTen(nowtime,(*it)))
			{
				it = m_updateMap.erase(it);
				retbool = false;
			}
			break;
		case auto_update_enum::TIME_TWENTYTHREE:
			if(!doUpdateTwentyThree(nowtime,(*it)))
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


bool AutoUpdateGame::doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodEvent.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))		//sPeriodEvent基准时间为0点，所以每0点开始，经过一个timelong为一个时间片，看是否在同一个时间片中
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

bool AutoUpdateGame::doUpdateFour(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSystem.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

bool AutoUpdateGame::doUpdateTwentyTwo(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSettle.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}


bool AutoUpdateGame::doUpdateTwentyThreeAndTen(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSimple.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}



bool AutoUpdateGame::doUpdateTwentyThree(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe)
{
	int timelong = m_periodMap[upe->GetCheckPeriod()];
	if(!sPeriodSimpleTwentyThree.IsInSameTimePeriod(nowtime,upe->GetLastUpdateTime(),timelong))
	{
		upe->SetLastUpdateTime(nowtime);
		upe->Execute();
	}
	return true;
}

void AutoUpdateGame::AddEvent( boost::function<void()> f, U32 start, int repeat /*= 1*/, U32 interval /*=0这里用秒*/ )
{
	m_eventTimer.AddEvent(f,start,repeat,interval);
}

void AutoUpdateGame::DoUpdatePer_Day_Zero()
{
	sPlayerPool.DoUpdatePer_Day_Zero();		//对每个玩家执行0基准重置操作

	for(int i=1; i<= NR_PLAYER_COUNTRY; i++)
	{
		sWorldMap.GetCountry(i)->updateNpcExpPerDay();
	}
}

void AutoUpdateGame::DoUpdatePer_FourHour_Zero()
{
	sPlayerPool.DoUpdatePer_FourHour_Zero();		//对每个玩家执行0基准4小时刷新操作
}

void AutoUpdateGame::DoUpdatePer_Day_TwentyThreeAndTen()
{
	sExploitMgr.InitializeExploitValue();
}

void AutoUpdateGame::DoUpdatePer_Day_TwentyThree()
{
	sExploitMgr.InitMilitaryPowerRankValue();
}

void AutoUpdateGame::DoUpdatePer_Hour_Zero()
{
	sPlayerPool.DoUpdatePer_Hour_Zero();

	for(int i=1; i<=NR_PLAYER_COUNTRY; i++)
	{
		sPalaceMgr(i)->UpdateOfficalAwardsPerHour();
	}
}

void AutoUpdateGame::DoUpdatePer_Second_Zero()
{

	PlayerMapLogic::UpdateCityEvent();
}

void AutoUpdateGame::DoUpdatePer_Minute_Zero()
{
	sActivityMgr.UpdateToEventLog();
	static bool flag[5] = {false,false,false,false,false};
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_STAR_BOX))
	{
		if(!flag[0])
		{
			flag[0] = true;
			PlayerActivityStarBoxDb::SendDeleteInfoToDb(pb::DATA_DEL);
		}
	}
	else
	{
		flag[0] = false;
	}
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_HERO_REWARD))
	{
		if(!flag[1]){
			flag[1] = true;
			GeneralRewardDB::SendDeleteInfoToDb(pb::DATA_DEL);
		}
	}
	else
	{
		flag[1] = false;
	}
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_COUNTRY_RANK))
	{
		if(!flag[2])
		{
			std::vector<int> vec;
			GetRankFromValue(vec);
			sPlayerPool.DoUpdatePer_Minute_Zero(vec);
			flag[2] = true;
			if(!vec.empty())
			{
				activity_occupy_city_god = 0;
				activity_occupy_city_human = 0;
				activity_occupy_city_worm = 0;
				ActivityOccupyCityDB::SendUpdateInfoToDb(pb::COUNTRY_GOD,activity_occupy_city_god);
				ActivityOccupyCityDB::SendUpdateInfoToDb(pb::COUNTRY_MAN,activity_occupy_city_human);
				ActivityOccupyCityDB::SendUpdateInfoToDb(pb::COUNTRY_WORM,activity_occupy_city_worm);
			}
		}
	}
	else
	{
		flag[2] = false;
	}
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_OCCUPATION_RANK))
	{
		if(!flag[3])
		{
			flag[3] = true;
			sActivityRankTableMgr.GiveOccupationRewardWhenActivityEnd();
		}
	}
	else
	{
		flag[3] = false;
	}
	if(!sActivityMgr.IsActivityActive(pb::ACTIVITY_ADD_SOLIDER_RANK))
	{
		if(!flag[4])
		{
			flag[4] = true;
			sActivityRankTableMgr.GiveAddSoldierRewardWhenActivityEnd();
		}
	}
	else
	{
		flag[4] = false;
	}
}
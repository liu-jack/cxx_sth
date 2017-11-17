#include "ActMgr.h"
#include "OS.h"
#include "../Map/Table/MapCountry.h"
#include "../Map/country/task/CountryTaskCity.h"
#include "Tournament/Tournament.h"
#include "OccupyCity/OccupyCity.h"
#include "Investment/Investment.h"
#include "Suppress/SuppressMonster.h"
#include "Defence/DefenceBorderland.h"
#include "Personal/PersonalTask.h"
#include "../Map/PlayerMapLogic.h"
#include "activity/ActMgr.pb.h"
#ifdef  _MMO_SERVER_
#include "../AutomaticUpdate/AutoUpdateGame.h"
#include "../GameServer.h"
#include "../server_client/EventLogServerClient.h"
#endif
#include "Opcode.pb.h"
//#include "ActMgr.pb.h"
#include "activity/ActMgr.pb.h"
#include "Logger.h"
#include "Enum.pb.h"
#include "InterServer.pb.h"
#include "def/EventLogDefine.h"
#include "Map/country/country.h"
#include "Map/WorldMap.h"
#include "common/utility/TimePeriod.h"
#include "common/auto_update_base/UpdateEventBase.h"

void ActMgr::Init()
{
	lastAct = (ActivityEnum)0;
	currentAct = (ActivityEnum)0;
	NextAct = (ActivityEnum)0;
	NextActStartHour = NULL_ACTIVE_HOUR;
	player_set.clear();
	isCrossWarCover = false;

	int hour = OS::TimeHour();	//当前小时数

	if (MapCountryQuest* table = sCountryTableMgr.RandNextTask(hour))
	{
		NextAct = (ActivityEnum)table->Id();
		NextActStartHour = hour;
		LLOG("init next act, id=%d, hour=%d",(int)sActMgr.NextAct,sActMgr.NextActStartHour);
	}
	else
	{
		PLOG("[Important Bug] cant get next country task");
	}

}

void ActMgr::_Update()
{
	int hour = OS::TimeHour();
    static int s_last_hour = hour;
    if (s_last_hour != hour) {
        OnEnterNexHour(s_last_hour = hour);
    }
}
void ActMgr::OnEnterNexHour(const int hour)
{
	//每小时更新记录三个国家城池个数
    PlayerMapLogic::_UpdateHaveCityCntPerHour(hour);

    UpdateActivity(hour);
}

#ifdef  _MMO_SERVER_
static void _Notify_Activity_End(uint32 type, uint64 timestart, uint64 timelong)
{
    pb::GS2C_ActMgr_End msg;
    msg.set_type(type);
    sGameSvr.BroadcastMsgToPlayer(pb::SMSG_ACTIVITY_END, msg);

	sActMgr.lastAct = sActMgr.currentAct;
	sActMgr.currentAct = ActMgr::Act_None;

	string outstrtime( "1970-01-01 08:00:00" );
	TimeT2String((time_t)timestart, outstrtime); 

	string outstrtime2( "1970-01-01 08:00:00" );
	TimeT2String((time_t)(timestart+timelong), outstrtime2); 

	if (MapCountryQuest* table = sCountryTableMgr.GetCountryTaskById(type))
	{
		sActMgr.AddEventLogFMT(pb::EVENT_PLAYER_COUNTRY_TASK,
			5,
			"i", type,
			"s", table->GetTaskComment(),
			"u", sActMgr.GetPlayerSetSize(),
			"s", outstrtime.c_str(),
			"s", outstrtime2.c_str()
			);
	}

	LLOG("record players in country task, num=%d",sActMgr.GetPlayerSetSize());

	//获取下一个活动信息
	int hour = OS::TimeHour();	//当前小时数

	if (MapCountryQuest* table = sCountryTableMgr.RandNextTask(hour))
	{
		sActMgr.NextAct = (ActMgr::ActivityEnum)table->Id();
		sActMgr.NextActStartHour = hour;
		LLOG("rand next act, id=%d, hour=%d",(int)sActMgr.NextAct,sActMgr.NextActStartHour);
	}
	else
	{
		PLOG("[Important Bug] cant get next country task");
	}

}
#endif

void ActMgr::AddPlayerInActivity(uint64 playerid)
{
	player_set.insert(playerid);
}

void ActMgr::AddEventLogFMT( pb::EVENT_ID eventID, int paramCount, ... )
{
#ifdef _MMO_SERVER_
	EventLogInfo evtMsg;
	evtMsg.event_id = eventID;
	evtMsg.account_id = 0;
	evtMsg.player_id = 0;


	va_list vlist;
	va_start(vlist, paramCount);

	sEventLogClient.SendEvent( evtMsg, paramCount, vlist, true);

	va_end(vlist);
#endif
}

//每小时刷新
void ActMgr::UpdateActivity(int hour)
{
	if(NextActStartHour == hour)
	{
		if (MapCountryQuest* table = sCountryTableMgr.GetCountryTaskById(NextAct))
		{
			int dayofweek = sOS.TimeDayOfWeek();
			if(dayofweek == auto_update_enum::Tuesday || dayofweek == auto_update_enum::Thursday || dayofweek == auto_update_enum::Saturday)
			{
				if(hour == 20)	//20点有可能开启跨服
				{
					if(Country* countrytest = sWorldMap.GetCountry(1))
					{
						if(countrytest->IsOpenCrossWar())	//开启了跨服战，则跳过此次国战
						{
							int hour = OS::TimeHour();	//当前小时数
							hour += 1;
							if (MapCountryQuest* table = sCountryTableMgr.RandNextTask(hour))
							{
								NextAct = (ActivityEnum)table->Id();
								NextActStartHour = hour;
								LLOG("init next act, id=%d, hour=%d",(int)sActMgr.NextAct,sActMgr.NextActStartHour);
							}
							else
							{
								PLOG("[Important Bug] cant get next country task");
							}
							return;
						}
					}
				}
			}

			bool isSucceed = false;
			const int lastSecond = table->GetLastMinute() * 60;	//持续时间
			switch (table->Id()) {
			case Attack_Defence_City:
				TakeAwardsBeforeStart(lastAct);
				isSucceed = sCountryTaskCity.Begin(lastSecond);
				break;
			case Tournament:
				TakeAwardsBeforeStart(lastAct);
				isSucceed = sActTournament.Begin(lastSecond);
				break;
			case Occupy_City:
				TakeAwardsBeforeStart(lastAct);
				isSucceed = sOccupyCity.Begin(lastSecond);
				break;
			case Suppress_Monster:
				TakeAwardsBeforeStart(lastAct);
				isSucceed = sSuppressMonster.Begin(lastSecond);
				break;
			case Investment:
				TakeAwardsBeforeStart(lastAct);
				isSucceed = sInvestment.Begin(lastSecond);
				break;
			case Defence_Borderland:
				TakeAwardsBeforeStart(lastAct);
				isSucceed = sDefenceBorderland.Begin(lastSecond);
				break;
			default:
				MMO_ASSERT(0);
				break;
			}
			if (isSucceed) {
				NLOG("task start!!! id=%d",table->Id());
				pb::GS2C_ActMgr_Begin msg;
				msg.set_type(table->Id());
#ifdef  _MMO_SERVER_
				uint64 timenow = sOS.TimeSeconds();
				sGameSvr.BroadcastMsgToPlayer(pb::SMSG_ACTIVITY_BEGIN, msg);
				//sAutoUpdateGame.AddEvent(boost::bind(_Notify_Activity_End, table->Id(),timenow,lastSecond), lastSecond);
				sPersonTask.AddCallBack_OnActEnd(boost::bind(_Notify_Activity_End,table->Id(),timenow,lastSecond));
#endif
				currentAct = (ActivityEnum)(table->Id());

				NextActStartHour = NULL_ACTIVE_HOUR;
				NextAct = (ActivityEnum)0;

				//开启个人任务
				sPersonTask.TakeAwardsBeforeStart();
				sPersonTask.Begin(lastSecond, table);
				player_set.clear();


			}
		}
	}
	

//    if (MapCountryQuest::IsStartHour(hour))		//当前整点有活动开始
//    {
//        if (MapCountryQuest* table = sCountryTableMgr.RandOneTask(hour))
//        {
//            bool isSucceed = false;
//            const int lastSecond = table->GetLastMinute() * 60;	//持续时间
//            switch (table->Id()) {
//            case Attack_Defence_City:
//				TakeAwardsBeforeStart(lastAct);
//                isSucceed = sCountryTaskCity.Begin(lastSecond);
//                break;
//			case Tournament:
//				TakeAwardsBeforeStart(lastAct);
//                isSucceed = sActTournament.Begin(lastSecond);
//                break;
//			case Occupy_City:
//				TakeAwardsBeforeStart(lastAct);
//                isSucceed = sOccupyCity.Begin(lastSecond);
//                break;
//			case Suppress_Monster:
//				TakeAwardsBeforeStart(lastAct);
//                isSucceed = sSuppressMonster.Begin(lastSecond);
//                break;
//			case Investment:
//				TakeAwardsBeforeStart(lastAct);
//                isSucceed = sInvestment.Begin(lastSecond);
//                break;
//			case Defence_Borderland:
//				TakeAwardsBeforeStart(lastAct);
//                isSucceed = sDefenceBorderland.Begin(lastSecond);
//                break;
//            default:
//                MMO_ASSERT(0);
//                break;
//            }
//            if (isSucceed) {
//				NLOG("task start!!! id=%d",table->Id());
//                pb::GS2C_ActMgr_Begin msg;
//                msg.set_type(table->Id());
//#ifdef  _MMO_SERVER_
//				uint64 timenow = sOS.TimeSeconds();
//                sGameSvr.BroadcastMsgToPlayer(pb::SMSG_ACTIVITY_BEGIN, msg);
//                sAutoUpdateGame.AddEvent(boost::bind(_Notify_Activity_End, table->Id(),timenow,lastSecond), lastSecond);
//#endif
//				currentAct = (ActivityEnum)(table->Id());
//
//				//开启个人任务
//				sPersonTask.TakeAwardsBeforeStart();
//				sPersonTask.Begin(lastSecond, table);
//
//				player_set.clear();
//
//            }
//        }
//	}
}

bool ActMgr::IsNowActivity()
{
	bool ret = false;
	if (MapCountryQuest* table = sCountryTableMgr.GetCountryTaskById(currentAct))
	{
		if(table)	//当前有活动
		{
			switch (table->Id()) {
				case Attack_Defence_City:
					ret = sCountryTaskCity.IsRunning();
					break;
				case Tournament:
					ret = sActTournament.m_isBegined;
					break;
				case Occupy_City:
					ret = sOccupyCity.m_isBegined;
					break;
				case Suppress_Monster:
					ret = sSuppressMonster.m_isBegined;
					break;
				case Investment:
					ret = sInvestment.m_isBegined;
					break;
				case Defence_Borderland:
					ret = sDefenceBorderland.m_isBegined;
					break;
				default:
					MMO_ASSERT(0);
					break;
			}
		}
	}
	return ret;
}

void ActMgr::GetActivityList(Player* player,pb::GS2C_ActMgr_Begin& msg)
{
	if(currentAct == Act_None)
		msg.set_type((int)lastAct);
	else
		msg.set_type((int)currentAct);
	msg.set_nexttype((int)NextAct);
	msg.set_nextstarthour(NextActStartHour);
}

void ActMgr::TakeAwardsBeforeStart(ActivityEnum tpy)
{
	switch (tpy) {
	case Attack_Defence_City:
		sCountryTaskCity.TakeAwardsBeforeStart();
		break;
	case Tournament:
		sActTournament.TakeAwardsBeforeStart();
		break;
	case Occupy_City:
		sOccupyCity.TakeAwardsBeforeStart();
		break;
	case Suppress_Monster:
		sSuppressMonster.TakeAwardsBeforeStart();
		break;
	case Investment:
		sInvestment.TakeAwardsBeforeStart();
		break;
	case Defence_Borderland:
		sDefenceBorderland.TakeAwardsBeforeStart();
		break;
	default:
		break;
	}
}
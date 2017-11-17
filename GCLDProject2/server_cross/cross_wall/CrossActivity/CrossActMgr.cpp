#include "CrossActMgr.h"
//#include "OS.h"
//#include "Opcode.pb.h"
//#include "Logger.h"
#include "CrossDestroy/CrossDestroy.h"
#include "CrossKill/CrossKill.h"
#include "object/Player.h"

void CrossActMgr::Init()
{
	m_PlayerTaskLevel.clear();
}

void CrossActMgr::StartActivity()
{
	m_PlayerTaskLevel.clear();
	sCrossDestroy.Begin();
	sCrossKill.Begin();
}

void CrossActMgr::EndActivity()
{
	sCrossDestroy.End();
	sCrossKill.End();
}
//void ActMgr::_Update()
//{
//	int hour = OS::TimeHour();
//	static int s_last_hour = hour;
//	if (s_last_hour != hour) {
//		OnEnterNexHour(s_last_hour = hour);
//	}
//}
//void ActMgr::OnEnterNexHour(const int hour)
//{
//	//每小时更新记录三个国家城池个数
//	PlayerMapLogic::_UpdateHaveCityCntPerHour(hour);
//
//	UpdateActivity(hour);
//}

//#ifdef  _MMO_SERVER_
//static void _Notify_Activity_End(uint32 type)
//{
//	pb::GS2C_ActMgr_End msg;
//	msg.set_type(type);
//	sGameSvr.BroadcastMsgToPlayer(pb::SMSG_ACTIVITY_END, msg);
//
//	sActMgr.lastAct = sActMgr.currentAct;
//	sActMgr.currentAct = ActMgr::Act_None;
//}
//#endif


////每小时刷新
//void ActMgr::UpdateActivity(int hour)
//{
//	if (MapCountryQuest::IsStartHour(hour))		//当前整点有活动开始
//	{
//		if (MapCountryQuest* table = sCountryTableMgr.RandOneTask())
//		{
//			bool isSucceed = false;
//			const int lastSecond = table->GetLastMinute() * 60;	//持续时间
//			switch (table->Id()) {
//			case Attack_Defence_City:
//				TakeAwardsBeforeStart(lastAct);
//				isSucceed = sCountryTaskCity.Begin(lastSecond);
//				break;
//			case Tournament:
//				TakeAwardsBeforeStart(lastAct);
//				isSucceed = sActTournament.Begin(lastSecond);
//				break;
//			case Occupy_City:
//				TakeAwardsBeforeStart(lastAct);
//				isSucceed = sOccupyCity.Begin(lastSecond);
//				break;
//			case Suppress_Monster:
//				TakeAwardsBeforeStart(lastAct);
//				isSucceed = sSuppressMonster.Begin(lastSecond);
//				break;
//			case Investment:
//				TakeAwardsBeforeStart(lastAct);
//				isSucceed = sInvestment.Begin(lastSecond);
//				break;
//			case Defence_Borderland:
//				TakeAwardsBeforeStart(lastAct);
//				isSucceed = sDefenceBorderland.Begin(lastSecond);
//				break;
//			default:
//				MMO_ASSERT(0);
//				break;
//			}
//			if (isSucceed) {
//				NLOG("task start!!! id=%d",table->Id());
//				pb::GS2C_ActMgr_Begin msg;
//				msg.set_type(table->Id());
//#ifdef  _MMO_SERVER_
//				sGameSvr.BroadcastMsgToPlayer(pb::SMSG_ACTIVITY_BEGIN, msg);
//				sAutoUpdateGame.AddEvent(boost::bind(_Notify_Activity_End, table->Id()), lastSecond);
//#endif
//				currentAct = (ActivityEnum)(table->Id());
//
//				//开启个人任务
//				sPersonTask.TakeAwardsBeforeStart();
//				sPersonTask.Begin(lastSecond, table);
//			}
//		}
//	}
//}

//void ActMgr::GetActivityList(Player* player,pb::GS2C_ActMgr_Begin& msg)
//{
//	if(currentAct == Act_None)
//		msg.set_type((int)lastAct);
//	else
//		msg.set_type((int)currentAct);
//}
//
void CrossActMgr::TakeAwardsOnEnd()
{
	sCrossKill.TakeAwardsOnEnd();
	sCrossDestroy.TakeAwardsOnEnd();
}

uint32 CrossActMgr::GetPlayerTaskLevel(Player& player)
{
	std::map<uint64, uint32>::iterator it = m_PlayerTaskLevel.find(player.GetGuid());
	if (it != m_PlayerTaskLevel.end())
	{
		return it->second;
	}
	else
	{
		m_PlayerTaskLevel[player.GetGuid()] = player.GetLevel();
		return m_PlayerTaskLevel[player.GetGuid()];
	}
}
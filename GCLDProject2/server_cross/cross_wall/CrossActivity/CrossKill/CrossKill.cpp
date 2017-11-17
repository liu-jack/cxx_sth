#include "CrossKill.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#include "../../table/CrossTableMgr.h"
#include "object/Player.h"
#include "Activity/Personal/PersonalQuestMgr.h"
#include "Trigger/trigger.h"
#include "reward/reward.h"
//#include "Activity/Personal/PersonalTask.h"
#include "session/PlayerPool.h"
#include "CrossWar.pb.h"
#include "../../CrossWarManager/CrossWarMgr.h"
#include "Map/country/country.h"
#include "../../CrossWorldMap.h"
#include "../CrossActMgr.h"

using namespace pb;
CrossKill::TaskSub::TaskSub()
{
	ZeroMemoryThis;
}

CrossKill::CrossKill()
{
	m_isBegined = false;

}

void CrossKill::SaveTo(Player& player, pb::GS2C_Cross_Kill_Task_Info& msg)
{
	if(m_isBegined)
	{
		if(IsPlayerInLst(player.GetGuid()))
		{
			TaskSub& playerone = m_playerLst[player.GetGuid()];
			msg.set_task_id(playerone.taskid);
			msg.set_progress(playerone.curCompletion);
			msg.set_is_take(playerone.isTakeAward);
		}
	}
}

bool CrossKill::Begin()
{
	if (m_isBegined) return false;	//重复开始
	PLOG("cross personal task start");
	m_isBegined = true;
	m_playerLst.clear();

	return true;
}

void CrossKill::End()
{
	m_isBegined = false;
}

bool CrossKill::IsPlayerInLst(uint64 playerid)
{
	std::map<uint64, TaskSub>::iterator it = m_playerLst.find(playerid);
	if (it != m_playerLst.end())
	{
		return true;
	}
	return false;
}

CrossKill::TaskSub& CrossKill::GetPlayerData(Player& player)
{
	std::map<uint64, TaskSub>::iterator it = m_playerLst.find(player.GetGuid());
	if (it == m_playerLst.end())
	{
		Table_Cross_One* crossOne = sCrossTableMgr.GetPQuestByLevel(sCrossActMgr.GetPlayerTaskLevel(player));

		LLOG("playerlevel=%d",player.GetGuid());
		
		TaskSub& playerone = m_playerLst[player.GetGuid()];

		if(crossOne)
		{

			playerone.taskid = crossOne->quest_last.front();
			playerone.pTable = crossOne;
			if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(playerone.taskid))
			{
				if(sTrigger.Check(&player,pquest->m_db.trigger_id))
				{
					playerone.taskType = (TaskEnum)pquest->m_db.type_id;
					playerone.subStageId = 1;
					playerone.target = pquest->m_TypeValue.front();
					playerone.curCompletion = 0;
					playerone.canTakeAward = false;
					playerone.isTakeAward = false;
				}
			}
			return playerone;
		}
		else
			ASSERT(0);

		return playerone;
	}

	return it->second;
}

void CrossKill::addProgress(TaskEnum tasktype, uint32 addnum, Player& player)
{
	if(m_isBegined)		//任务开启状态
	{
		if(Country* country = sCrossWorldMap.GetCountry(player.GetCountryId()))
		{
			if(!country->IsCrossCountryOccypied())
			{
				TaskSub& playerone = GetPlayerData(player);

				if(tasktype == playerone.taskType)		//是增加该任务进度的
				{
					playerone.curCompletion += addnum;
					if(playerone.curCompletion >= playerone.target)
						playerone.canTakeAward = true;
					sCrossWarMgr.AddKillCnt(player.GetGuid(),player.GetCountryId(),addnum);
				}
			}
		}
	}
}

void CrossKill::TakeAwards(Player&player, uint8 taskid, pb::GS2C_Take_Kill_Task_Award_Ret& msg)
{
	msg.set_ret(Cro_Common_Err);
	if(IsPlayerInLst(player.GetGuid()))
	{
		TaskSub& playerone = GetPlayerData(player);

		if(taskid == playerone.taskid)
		{
			if(!playerone.isTakeAward)
			{
				if(playerone.canTakeAward)
				{
					//领取完成奖励
					if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest((int)playerone.taskid))
					{
						sReward.Change(player,pquest->m_Rewards);
					}
					playerone.isTakeAward = true;
					msg.set_ret(Cro_Common_Suc);

					int substagecount = 0;
					for(std::vector<int>::iterator it = playerone.pTable->quest_last.begin(); it != playerone.pTable->quest_last.end(); it++)
					{
						if(playerone.subStageId == substagecount)
						{
							if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(*it))
							{
								if(playerone.curCompletion > playerone.target)
									playerone.curCompletion -= playerone.target;
								else
									playerone.curCompletion = 0;
								playerone.canTakeAward = false;
								playerone.isTakeAward = false;
								playerone.taskid = pquest->m_db.id;
								playerone.target = pquest->m_TypeValue.front();
								playerone.taskType = (TaskEnum)pquest->m_db.type_id;

								if(playerone.curCompletion >= playerone.target)
									playerone.canTakeAward = true;

								playerone.subStageId++;
							}
							break;
						}
						substagecount++;
					}
				}
				else
					msg.set_ret(Cro_War_Task_Not_Finished);
			}
			else
				msg.set_ret(Cro_War_Reward_IsTake);

			GS2C_Cross_Kill_Task_Info* taskone = msg.mutable_task_info();
			taskone->set_task_id(taskid);
			taskone->set_progress(playerone.curCompletion);
			taskone->set_is_take(playerone.isTakeAward);
		}
		else
			msg.set_ret(Cro_War_Task_Id_Dismatch);
	}
	else
		msg.set_ret(Cro_war_Player_Not_In_Lst);
}

void CrossKill::TakeAwardsOnEnd()
{
	for(std::map<uint64, TaskSub>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); it++)
	{
		TaskSub& playerone = it->second;
		uint64 playerid = it->first;
		if(Player* player = sPlayerPool.GetByPlayerId(playerid))
		{
			if(playerone.canTakeAward && !playerone.isTakeAward)
			{
				//领取完成奖励
				if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest((int)playerone.taskid))
				{
					sReward.Change(*player,pquest->m_Rewards);
				}
				playerone.isTakeAward = true;
			}
		}
	}
	m_playerLst.clear();
}

uint32 CrossKill::GetTaskId(Player& player)
{
	TaskSub& playerone = GetPlayerData(player);
	return playerone.taskid;
}
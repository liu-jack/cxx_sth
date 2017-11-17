#include "CrossDestroy.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#include "../../table/CrossTableMgr.h"
#include "object/Player.h"
#include "Activity/Personal/PersonalQuestMgr.h"
#include "Trigger/trigger.h"
//#include "Activity/Personal/PersonalTask.h"
#include "reward/reward.h"
//#include "datastruct/struct_cross_kill_quest.h"
#include "session/PlayerPool.h"
#include "CrossWar.pb.h"
#include "../CrossActMgr.h"

using namespace pb;
CrossDestroy::TaskPlayer::TaskPlayer()
{
	ZeroMemoryThis;
}

CrossDestroy::CrossDestroy()
{
	m_isBegined = false;

}

void CrossDestroy::SaveTo(Player& player, pb::GS2C_Cross_Destroy_Task_Infos& msg)
{
	if(m_isBegined)
	{
		//if(IsPlayerInLst(player.GetGuid()))
		//{
			TaskPlayer& playerone = GetPlayerData(player);
			for(int i=0; i<3; i++)
			{
				GS2C_Cross_Destroy_Task_Info* taskonemsg = msg.add_task_list();
				TaskSub& taskone = playerone.task[i];
				taskonemsg->set_task_id(taskone.taskid);
				taskonemsg->set_progress(taskone.curCompletion);
				taskonemsg->set_is_take(taskone.isTakeAward);
			}

		//}
	}
}

bool CrossDestroy::Begin()
{
	if (m_isBegined) return false;	//重复开始
	PLOG("cross personal task start");
	m_isBegined = true;
	m_playerLst.clear();

	return true;
}

void CrossDestroy::End()
{
	m_isBegined = false;
}

bool CrossDestroy::IsPlayerInLst(uint64 playerid)
{
	std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(playerid);
	if (it != m_playerLst.end())
	{
		return true;
	}
	return false;
}

CrossDestroy::TaskPlayer& CrossDestroy::GetPlayerData(Player& player)
{
	std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
	if (it == m_playerLst.end())
	{
		TaskPlayer& playerone = m_playerLst[player.GetGuid()];

		Table_Cross_One* crossOne = sCrossTableMgr.GetPQuestByLevel(sCrossActMgr.GetPlayerTaskLevel(player));

		if(crossOne)
		{
			playerone.task[0].taskid = crossOne->GetQuest1();
			playerone.task[1].taskid = crossOne->GetQuest2();
			playerone.task[2].taskid = crossOne->GetQuest3();

			for(int i=0; i<3; i++)
			{
				if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(playerone.task[i].taskid))
				{
					if(sTrigger.Check(&player,pquest->m_db.trigger_id))
					{
						TaskSub& taskone = playerone.task[i];
						taskone.canTakeAward = false;
						taskone.curCompletion = 0;
						taskone.isTakeAward = false;
						taskone.target = pquest->m_TypeValue.front();
						taskone.taskType = (TaskEnum)pquest->m_db.type_id;
					}
				}
			}
		}
		return playerone;
	}

	return it->second;
}

void CrossDestroy::addProgress(TaskEnum tasktype, uint32 addnum, Player& player)
{
	if(m_isBegined)		//任务开启状态
	{

		TaskPlayer& playerone = GetPlayerData(player);

		for(int i=0; i<3; i++)
		{
			if(tasktype == playerone.task[i].taskType)		//是增加该任务进度的
			{
				playerone.task[i].curCompletion += addnum;
				if(playerone.task[i].curCompletion >= playerone.task[i].target)
					playerone.task[i].canTakeAward = true;

				GS2C_Cross_Destroy_Task_Info msg;
				msg.set_task_id(playerone.task[i].taskid);
				msg.set_progress(playerone.task[i].curCompletion);
				msg.set_is_take(playerone.task[i].isTakeAward);
				player.Send(SMSG_CROSS_DESTROY_TASK_INFO_UPDATE,msg);
			}
		}
	}
}

void CrossDestroy::TakeAwards(Player&player, uint8 taskid,GS2C_Take_Destroy_Task_Award_Ret& msg)
{
	msg.set_ret(Cro_Common_Err);
	if(IsPlayerInLst(player.GetGuid()))
	{
		TaskPlayer& playerone = GetPlayerData(player);
		for(int i=0; i<3; i++)
		{
			if(taskid == playerone.task[i].taskid)
			{
				if(!playerone.task[i].isTakeAward)
				{
					if(playerone.task[i].canTakeAward)
					{
						//领取完成奖励
						if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest((int)playerone.task[i].taskid))
						{
							sReward.Change(player,pquest->m_Rewards);

							//SC2G_Update_Award_Change updateMsg;

						}
						playerone.task[i].isTakeAward = true;
						msg.set_ret(Cro_Common_Suc);
					}
					else
						msg.set_ret(Cro_War_Task_Not_Finished);
				}
				else
					msg.set_ret(Cro_War_Reward_IsTake);

				GS2C_Cross_Destroy_Task_Info* taskone = msg.mutable_task_info();
				taskone->set_task_id(taskid);
				taskone->set_progress(playerone.task[i].curCompletion);
				taskone->set_is_take(playerone.task[i].isTakeAward);
			}
		}
	}
	else
		msg.set_ret(Cro_war_Player_Not_In_Lst);
}

void CrossDestroy::TakeAwardsOnEnd()
{
	for(std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); it++)
	{
		TaskPlayer& playerone = it->second;
		uint64 playerid = it->first;
		if(Player* player = sPlayerPool.GetByPlayerId(playerid))
		{
			for(int i=0; i<3; i++)
			{
				if(playerone.task[i].canTakeAward && !playerone.task[i].isTakeAward)
				{
					//领取完成奖励
					if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest((int)playerone.task[i].taskid))
					{
						sReward.Change(*player,pquest->m_Rewards);
					}
					playerone.task[i].isTakeAward = true;
				}
			}
		}
	}
	m_playerLst.clear();
}
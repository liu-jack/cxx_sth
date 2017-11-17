#include "PersonalTask.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#ifdef _MMO_SERVER_
//#include "../../GameServer.h"
#include "../../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "Map/country/country.h"
#include "Map/WorldMap.h"
#include "object/Player.h"
#include "PersonalQuestMgr.h"
#include "Trigger/trigger.h"
#include "reward/reward.h"
#include "session/PlayerPool.h"

using namespace pb;
PersonTask::TaskPlayer::TaskPlayer()
{
	ZeroMemoryThis;
}

PersonTask::PersonTask()
{
	m_isBegined = false;
	for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
	{
		m_isCountryOpen[i] = false;
	}
}

void PersonTask::SaveTo(pb::OneTaskInfo& msg, Player& player, uint32 taskid)
{
	if(isPlayerInList(player.GetGuid()))
	{
		TaskPlayer& playerone = GetPlayerData(player);
		for(int i=0; i<3; i++)
		{
			TaskSub& taskone = playerone.task[i];
			if(taskone.taskid == taskid)
			{
				msg.set_stageid(1);
				msg.set_taskid(taskid);
				msg.set_curcompletion(taskone.curCompletion);
				msg.set_istakeaward(taskone.isTakeAward);
				return;
			}
		}

		if(playerone.task_second.taskid == taskid)
		{
			msg.set_stageid(2);
			msg.set_taskid(taskid);
			msg.set_curcompletion(playerone.task_second.curCompletion);
			msg.set_istakeaward(playerone.task_second.isTakeAward);
			return;
		}
	}
}

void PersonTask::SaveTo(pb::GS2C_PersonalTask_Info& msg, Player& player)
{
	if(!m_isCountryOpen[player.GetCountryId()-1])	//不满足条件，则个人任务不开启
	{
		msg.set_is_begin(false);
		LLOG("send personal task ask false");
		return;
	}
	msg.set_is_begin(m_isBegined);
	LLOG("send personal task ask %d",m_isBegined);
	TaskPlayer& playerone = GetPlayerData(player);
	msg.set_curstage(playerone.nowStage);
	msg.set_cursubstage(playerone.nowSubStage);
	msg.set_taskbegintime(m_startTime);
	for(int i=0; i<3; i++)
	{
		OneTaskInfo* taskone = msg.add_tasks();
		SaveTo(*taskone,player,playerone.task[i].taskid);
	}
	OneTaskInfo* taskone = msg.add_tasks();
	SaveTo(*taskone,player,playerone.task_second.taskid);
}

bool PersonTask::Begin(int lastSecond, MapCountryQuest* table)
{
	if (m_isBegined) return false;	//重复开始
	PLOG("personal task start");
	m_isBegined = true;
	m_playerLst.clear();
	tasklst.clear();
	taskSecond.clear();
	m_startTime = sOS.TimeSeconds();
	for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
	{
		Country* country = sWorldMap.GetCountry(i + 1);
		if(country->GetLevel()>=GET_BASE_DEF_INT( pb::BD_COUNTRY_UNLOCK_PERSONAL_QUEST))
			m_isCountryOpen[i] = true;
		else
			m_isCountryOpen[i] = false;
	}

	table->GetTaskLst(tasklst);
	table->GetTaskSecond(taskSecond);

#ifdef _MMO_SERVER_
	sAutoUpdateGame.AddEvent(boost::bind(&PersonTask::End, this), lastSecond);		//持续时间到了之后结束
#endif

	return true;
}

void PersonTask::End()
{
	if(!m_isBegined) return;	//没有开始

	m_isBegined = false;

	GS2C_PersonalTask_Info msg;
	msg.set_is_begin(m_isBegined);
	LLOG("send end personal task");
	sPlayerPool.BroadcastMsgToAll(SMSG_PERSONAL_TASK, msg);

	//通知活动结束
	for (CallBackEnd::iterator it = m_ActEnd.begin(); it != m_ActEnd.end(); ++it)
	{
		(*it)();
	}
}

void PersonTask::AddCallBack_OnActEnd(const boost::function<void()>& func)
{
	m_ActEnd.push_back(func);
}

//在调用该函数前先判断能否开启个人任务
PersonTask::TaskPlayer& PersonTask::GetPlayerData(Player& player)
{
	std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
	if (it == m_playerLst.end())
	{
		TaskPlayer& playerone = m_playerLst[player.GetGuid()];
		playerone.nowStage = 1;	//第一阶段
		playerone.nowSubStage = 0;	//第0子阶段

		std::random_shuffle(tasklst.begin(), tasklst.end());
		
		int index = 0;
		for(std::vector<int>::iterator it = tasklst.begin();
				it != tasklst.end(); it++)
		{
			if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(*it))
			{
				if(sTrigger.Check(&player,pquest->m_db.trigger_id))
				{
					TaskSub& taskone = playerone.task[index++];
					taskone.taskid = pquest->m_db.id;
					taskone.taskType = (TaskEnum)pquest->m_db.type_id;
					taskone.target = pquest->m_TypeValue.front();
					taskone.curCompletion = 0;
				}

				if(index >=3)	//只查找三个
					break;
			}
		}

		if(const Table_PQuest* pquest2 = sPersonalQuestMgr.GetPQuest(taskSecond.front()))
		{
			if(sTrigger.Check(&player,pquest2->m_db.trigger_id))
			{
				playerone.task_second.taskid = pquest2->m_db.id;
				playerone.task_second.taskType = (TaskEnum)pquest2->m_db.type_id;
				playerone.task_second.target = pquest2->m_TypeValue.front();
				playerone.task_second.curCompletion = 0;
			}
		}

		PLOG("get one player task. 1:taskid=%d,taskGoal=%d. 2:taskid=%d,taskGoal=%d. 3:taskid=%d,taskGoal=%d. second:taskid=%d,taskGoal=%d"
						,playerone.task[0].taskid,playerone.task[0].target,playerone.task[1].taskid,playerone.task[1].target
						,playerone.task[2].taskid,playerone.task[2].target,playerone.task_second.taskid,playerone.task_second.target);
		return playerone;
	}

	return it->second;
}

bool PersonTask::isPlayerInList(uint64 playerid)
{
	std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(playerid);
	if (it != m_playerLst.end())
	{
		return true;
	}
	return false;
}

void PersonTask::addProgress(TaskEnum tasktype, uint32 addnum, Player& player)
{
	if(m_isBegined)		//任务开启状态
	{
		if(m_isCountryOpen[player.GetCountryId()-1])	//该玩家所在国家满足条件
		{
			TaskPlayer& playerone = GetPlayerData(player);
			if(playerone.nowStage == 1)	//第一阶段
			{
				for(int i=0; i<3; i++)
				{
					if(tasktype == playerone.task[i].taskType)		//是增加该任务进度的
					{
						playerone.task[i].curCompletion += addnum;
						if(playerone.task[i].curCompletion >= playerone.task[i].target)
							playerone.task[i].canTakeAward = true;

						pb::OneTaskInfo taskone;
						taskone.set_stageid(playerone.nowStage);
						taskone.set_taskid(playerone.task[i].taskid);
						taskone.set_curcompletion(playerone.task[i].curCompletion);
						taskone.set_istakeaward(playerone.task[i].isTakeAward);
						player.Send(SMSG_PERSONAL_TASK_ONE,taskone);
					}
				}
			}
			else	//第二阶段
			{
				if(tasktype == KILL_IN_COUNTRY)		//是增加第二阶段的进度的
				{
					playerone.task_second.curCompletion += addnum;
					if(playerone.task_second.curCompletion >= playerone.task_second.target)
						playerone.task_second.canTakeAward = true;

					pb::OneTaskInfo taskone;
					taskone.set_stageid(playerone.nowStage);
					taskone.set_taskid(playerone.task_second.taskid);
					taskone.set_curcompletion(playerone.task_second.curCompletion);
					taskone.set_istakeaward(playerone.task_second.isTakeAward);
					player.Send(SMSG_PERSONAL_TASK_ONE,taskone);
				}
			}
		}
	}
}

void PersonTask::_TakeRewards(Player&player, uint8 taskid, pb::GS2C_PersonalTask_Take_Award& msg)
{
	msg.set_issuccess(false);
	msg.set_errret(pb::TaskErr);
	if(isPlayerInList(player.GetGuid()))
	{
		TaskPlayer& playerone = GetPlayerData(player);
		if(playerone.nowStage == 1)
		{
			bool isfound = false;
			for(int i=0; i<3; i++)
			{
				if(taskid == playerone.task[i].taskid)
				{
					isfound = true;
					if(playerone.task[i].canTakeAward)
					{
						if(!playerone.task[i].isTakeAward)
						{
							//领取完成奖励
							if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest((int)playerone.task[i].taskid))
							{
								sReward.Change(player,pquest->m_Rewards);
								playerone.task[i].isTakeAward = true;
								msg.set_issuccess(true);
							}
							else
								msg.set_errret(pb::TaskNotFind);
						}
						else
							msg.set_errret(pb::rewardHasTaken);
					}
					else
						msg.set_errret(pb::TaskIsNotFinished);
					break;
				}
			}

			if(!isfound)
				msg.set_errret(pb::StageNotMatch);

			if(playerone.task[0].isTakeAward && playerone.task[1].isTakeAward && playerone.task[2].isTakeAward)	//三个奖励都领取了
			{
				if(m_isBegined)	//任务没有结束，则开启第二阶段
				{
					playerone.nowStage = 2;
					playerone.nowSubStage = 1;
				}
			}
		}
		else
		{
			if(taskid == playerone.task_second.taskid)
			{
				if(playerone.task_second.canTakeAward)
				{
					if(!playerone.task_second.isTakeAward)
					{
						//领取完成奖励
						if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest((int)playerone.task_second.taskid))
						{
							sReward.Change(player,pquest->m_Rewards);
							playerone.task_second.isTakeAward = true;
							msg.set_issuccess(true);
						}
						else
							msg.set_errret(pb::TaskNotFind);

						playerone.task_second.isTakeAward = true;

						if(m_isBegined)	//任务没有结束，则开启下一阶段
						{
							int substagecount = 0;
							for(std::vector<int>::iterator it = taskSecond.begin(); it != taskSecond.end(); it++)
							{
								if(substagecount == playerone.nowSubStage)//if(*it == playerone.task_second.taskid)
								{
									//if(++it != taskSecond.end())
									//{
										if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(*it))
										{
											playerone.task_second.canTakeAward = false;
											if(playerone.task_second.curCompletion > playerone.task_second.target)
												playerone.task_second.curCompletion -= playerone.task_second.target;
											else
												playerone.task_second.curCompletion = 0;
											playerone.task_second.isTakeAward = false;
											playerone.task_second.taskid = pquest->m_db.id;
											playerone.task_second.target = pquest->m_TypeValue.front();
											playerone.task_second.taskType = (TaskEnum)pquest->m_db.type_id;
											playerone.nowSubStage++;
										}
									//}
									break;
								}
								substagecount++;
							}
						}
					}
					else
						msg.set_errret(pb::rewardHasTaken);
				}
				else
					msg.set_errret(pb::TaskIsNotFinished);
			}
			else
				msg.set_errret(pb::StageNotMatch);
		}
	}
	else
		msg.set_errret(pb::NoRecord);
}

void PersonTask::TakeAwardsBeforeStart()
{
	for(std::map<uint64, TaskPlayer>::iterator ite = m_playerLst.begin(); ite != m_playerLst.end(); ite++)
	{
		TaskPlayer& playerone = ite->second;
		if(Player* palyer = sPlayerPool.GetByPlayerId(ite->first))
		{
			if(playerone.nowStage == 1)	//第一阶段
			{
				for(int i=0; i<3; i++)
				{
					TaskSub& tasksub = playerone.task[i];
					if(tasksub.canTakeAward && !tasksub.isTakeAward)
					{
						if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(tasksub.taskid))
						{
							sReward.Change(*palyer,pquest->m_Rewards);
						}
					}
				}
			}
			else
			{
				TaskSub& tasksub = playerone.task_second;
				if(tasksub.canTakeAward && !tasksub.isTakeAward)
				{
					if(const Table_PQuest* pquest = sPersonalQuestMgr.GetPQuest(tasksub.taskid))
					{
						sReward.Change(*palyer,pquest->m_Rewards);
					}
				}
			}
		}
	}
	m_playerLst.clear();
}
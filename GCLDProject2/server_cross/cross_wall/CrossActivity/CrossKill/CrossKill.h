#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "Activity/Personal/PersonalTask.h"
class Player;
class Table_Cross_One;
//enum TaskEnum;
namespace pb
{
	class GS2C_Cross_Kill_Task_Info;
	class GS2C_Take_Kill_Task_Award_Ret;
}

class CrossKill
{
public:
	struct TaskSub{
		TaskSub();
		uint32 taskid;			//任务编号
		TaskEnum taskType;		//任务类型
		uint32		subStageId;	//阶段id
		uint32		target;		//任务目标
		uint32		curCompletion;//任务完成度
		bool		canTakeAward;//奖励能否领取
		bool		isTakeAward;//奖励是否已领取
		Table_Cross_One* pTable;//任务信息
	};

	std::map<uint64, TaskSub>    m_playerLst;		//玩家列表
	bool	m_isBegined;

public:
	CrossKill();
	static CrossKill& Instance(){ static CrossKill T; return T; }

	bool Begin();
	void End();

	void SaveTo(Player& player, pb::GS2C_Cross_Kill_Task_Info& msg);

	bool IsPlayerInLst(uint64 playerid);
	TaskSub& GetPlayerData(Player& player);

	void addProgress(TaskEnum tasktype, uint32 addnum, Player& player);	//增加任务进度

	void TakeAwards(Player&player, uint8 taskid, pb::GS2C_Take_Kill_Task_Award_Ret& msg);
	void TakeAwardsOnEnd();

	uint32 GetTaskId(Player& player);
};

#define sCrossKill CrossKill::Instance()
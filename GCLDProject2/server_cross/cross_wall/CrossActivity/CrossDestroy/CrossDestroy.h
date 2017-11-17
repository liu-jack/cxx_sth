#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "Activity/Personal/PersonalTask.h"
class Player;
//enum TaskEnum;
namespace pb
{
	class GS2C_Cross_Destroy_Task_Infos;
	class GS2C_Take_Destroy_Task_Award_Ret;
}

class CrossDestroy
{
public:
	struct TaskSub{
		uint32		taskid;		//任务编号
		TaskEnum	taskType;	//任务类型
		uint32		target;		//任务目标
		uint32		curCompletion;//任务完成度
		bool		canTakeAward;//奖励能否领取
		bool		isTakeAward;//奖励是否已领取
	};

	struct TaskPlayer { //参与玩家
		TaskPlayer();
		TaskSub task[3];		//三个小任务
	};

	std::map<uint64, TaskPlayer>    m_playerLst;		//玩家列表
	bool	m_isBegined;

public:
	CrossDestroy();
	static CrossDestroy& Instance(){ static CrossDestroy T; return T; }

	bool Begin();
	void End();

	void SaveTo(Player& player, pb::GS2C_Cross_Destroy_Task_Infos& msg);

	bool IsPlayerInLst(uint64 playerid);
	TaskPlayer& GetPlayerData(Player& player);
	void addProgress(TaskEnum tasktype, uint32 addnum, Player& player);	//增加任务进度

	void TakeAwards(Player&player, uint8 taskid, pb::GS2C_Take_Destroy_Task_Award_Ret& msg);
	void TakeAwardsOnEnd();
};

#define sCrossDestroy CrossDestroy::Instance()
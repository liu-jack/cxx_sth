#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "Map/Table/MapCountry.h"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "Base.h"
#include "boost/function.hpp"

class Player;
namespace pb
{
	class OneTaskInfo;
	class GS2C_PersonalTask_Info;
	class GS2C_PersonalTask_Take_Award;
}

enum TaskEnum{
	TASK_NULL,
	KILL_IN_COUNTRY,	//国战杀敌
	ATTRACT_IN_COUNTRY,	//国战诱敌
	RUSH_IN_COUNTRY,	//国战突进
	ADD_SOLIDER_IN_COUNTRY,	//国战增兵
	TOURNAMENT_IN_COUNTRY,//比武次数
	CASTING_TIMES,			//神器铸造次数
	INVEST_TIMES,			//投资次数
	OCCUPY_TIMES,			//攻占城池数
};

class PersonTask
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
		uint8 nowStage;	//当前进行的阶段
		uint32 nowSubStage;	//第二阶段的小阶段
		TaskSub task[3];		//一阶段小任务 三个
		TaskSub task_second;//二阶段任务
	};
	std::map<uint64, TaskPlayer>    m_playerLst;		//玩家列表

	bool	m_isBegined;		
	uint64	m_startTime;
	bool	m_isCountryOpen[3];		//三个国家是否能开始个人任务
	std::vector<int> tasklst;
	std::vector<int> taskSecond;

private:
	typedef std::vector<boost::function<void()> >CallBackEnd;
	CallBackEnd m_ActEnd;

public:
	PersonTask();
	static PersonTask& Instance(){ static PersonTask T; return T; }
	
	bool Begin(int lastSecond, MapCountryQuest* table);
	void End();

	void TakeAwardsBeforeStart();					//下一个任务开始之前将奖励都领取了

	void GetPersonalTask();
	TaskPlayer& GetPlayerData(Player& player);
	bool isPlayerInList(uint64 playerid);

	void addProgress(TaskEnum tasktype, uint32 addnum, Player& player);	//增加任务进度

	void _TakeRewards(Player&player, uint8 taskid, pb::GS2C_PersonalTask_Take_Award& msg);//发送获取奖励
	
	void AddCallBack_OnActEnd(const boost::function<void()>& func);

public:
	void SaveTo(pb::OneTaskInfo& msg, Player& player, uint32 taskid);
	void SaveTo(pb::GS2C_PersonalTask_Info& msg, Player& player);

};

#define sPersonTask PersonTask::Instance()
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __DAILY_QUEST_TABLE_MGR__H
#define __DAILY_QUEST_TABLE_MGR__H

#include "game_data/datastruct/struct_daily_quest.h"
#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "DailyQuest/daily_quest_def.h"
class Player;
class LootList;
class DailyQuest
{
private:
	const struct DB_DailyQuest& db_dailyquest_;
public:
	DailyQuest(const DB_DailyQuest& db_dailyquest);
	uint32 Id() const;
	uint32 TypeId() const;
	uint32 Value1() const;
	uint32 Value2() const;
	uint32 QuestRewardScore() const;
	uint32 NameId() const;
	uint32 SkipId() const;
	uint32 TriggerId() const;
};


class DailyQuestReward
{
private:
	const struct DB_DailyQuestReward &db_dailyquestreward_;
public:
	DailyQuestReward(const DB_DailyQuestReward& db_dailyquestreward);
	uint32 Id() const;
	uint32 RewardScore() const;
	uint32 UnlockLevel() const;

};

typedef PtrMap<uint32,DailyQuest> DailyQuestMap;
typedef PtrMap<uint32,DailyQuestReward> DailyQuestRewardMap;
class DailyQuestTableMgr : public SimpleSingleton<DailyQuestTableMgr>
{
private:
	DailyQuestMap daily_quest_map_;
	DailyQuestRewardMap daily_quest_reward_map_;
public:
	DailyQuestTableMgr();
	~DailyQuestTableMgr();
	void Init();
	static DailyQuestTableMgr& Instance(){ static DailyQuestTableMgr T; return T; }	//静态单例
	bool GetQuestReward(Player& player,int VictimType,LootList& lootlist,const uint32 id);//完成任务奖励

	uint32 GetRewardIdFromScore(const uint32 score) const;//useless

	const DailyQuest* GetDailyQuest(uint32 id) const;
	const DailyQuestReward* GetDailyQuestReward(uint32 id) const;
	const DailyQuestMap& GetDailyQuestMap() const;
	const uint32 GetQuestIdFromQuestType(const DailyQuestType type,const uint32 city_id) const;
};

#define sDailyQuestTableMgr DailyQuestTableMgr::Instance()

#endif
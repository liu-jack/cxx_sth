/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "DailyQuestTableMgr.h"
#include "object/Player.h"
#include "Loot/LootList.h"

DailyQuest::DailyQuest( const DB_DailyQuest& db_dailyquest )
:db_dailyquest_(db_dailyquest)
{

}
uint32 DailyQuest::Id() const
{
	return db_dailyquest_.id;
}

uint32 DailyQuest::TypeId() const
{
	return db_dailyquest_.type_id;
}

uint32 DailyQuest::Value1() const
{
	return db_dailyquest_.val1;
}

uint32 DailyQuest::Value2() const
{
	return db_dailyquest_.val2;
}

uint32 DailyQuest::QuestRewardScore() const
{
	return db_dailyquest_.reward_score;
}

uint32 DailyQuest::NameId() const
{
	return db_dailyquest_.name_id;
}

uint32 DailyQuest::SkipId() const
{
	return db_dailyquest_.skip_id;
}

uint32 DailyQuest::TriggerId() const
{
	return db_dailyquest_.trigger_id;
}

DailyQuestReward::DailyQuestReward( const DB_DailyQuestReward& db_dailyquestreward )
:db_dailyquestreward_(db_dailyquestreward)
{

}

uint32 DailyQuestReward::Id() const
{
	return db_dailyquestreward_.id;
}

uint32 DailyQuestReward::RewardScore() const
{
	return db_dailyquestreward_.reward_score;
}

uint32 DailyQuestReward::UnlockLevel() const
{
	return db_dailyquestreward_.unlock_level;
}


DailyQuestTableMgr::DailyQuestTableMgr()
{

}

DailyQuestTableMgr::~DailyQuestTableMgr()
{

}

void DailyQuestTableMgr::Init()
{
	FOREACH_DB_ITEM(dailyquest,DB_DailyQuest)
	{
		uint32 Id = dailyquest->id;
		DailyQuest * dailyQuest = new DailyQuest(*dailyquest);
		daily_quest_map_[Id] = dailyQuest;
	}

	FOREACH_DB_ITEM(dailyquestreward,DB_DailyQuestReward)
	{
		uint32 Id = dailyquestreward->id;
		DailyQuestReward * dailyQuestReward = new DailyQuestReward(*dailyquestreward);
		daily_quest_reward_map_[Id] = dailyQuestReward;
	}
}

bool DailyQuestTableMgr::GetQuestReward( Player& player,int VictimType,LootList& lootlist,const uint32 id)
{
	LootParam param;
	param.victim_type = VictimType;
	if(id == 0) return false;
	param.victim_id = id;
	param.victim_lv = player.GetLevel();
	param.player_vip_lv = player.GetVipLevel();
	return sLootMgr.FillLootList(&player,param, lootlist,true);
}

const DailyQuest* DailyQuestTableMgr::GetDailyQuest( uint32 id ) const
{
	return daily_quest_map_.pfind(id);
}

const DailyQuestReward* DailyQuestTableMgr::GetDailyQuestReward( uint32 id ) const
{
	return daily_quest_reward_map_.pfind(id);
}

uint32 DailyQuestTableMgr::GetRewardIdFromScore( const uint32 score ) const
{
	for(DailyQuestRewardMap::const_iterator CIter = daily_quest_reward_map_.begin();CIter != daily_quest_reward_map_.end();++CIter)
	{
		DailyQuestReward * dailyQuestReward = CIter->second;
		if(dailyQuestReward->RewardScore() == score)
			return dailyQuestReward->Id();
	}
	return 0;
}

const DailyQuestMap& DailyQuestTableMgr::GetDailyQuestMap() const
{
	return daily_quest_map_;
}

const uint32 DailyQuestTableMgr::GetQuestIdFromQuestType( const DailyQuestType type ,const uint32 city_id) const
{
	DailyQuestMap::const_iterator iter = daily_quest_map_.begin();
	for(;iter != daily_quest_map_.end();++iter)
	{
		if(UNLIKELY(city_id != 0))
		{
			if(type == iter->second->TypeId())
			{
				if(type == OCCUPY_SOME_CITY && city_id == iter->second->Value2())
				{
					return iter->first;
				}
			}
		}
		else 
		{
			if(type == iter->second->TypeId())
				return iter->first;
		}
	}
	return 0;
}
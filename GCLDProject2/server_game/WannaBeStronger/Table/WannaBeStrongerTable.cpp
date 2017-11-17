
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#include "WannaBeStronger/Table/WannaBeStrongerTable.h"
#include "utility/Utility.h"
#include "DbTool.h"
StrongerQuest::StrongerQuest(const DB_StrongerQuest& db)
:db_(db)
{
}

uint32 StrongerQuest::QuestId() const
{
	return db_.id;
}

uint32 StrongerQuest::QuestGroup() const
{
	return db_.quest_group;
}

uint32 StrongerQuest::QuestType() const
{
	return db_.quest_type;
}

uint32 StrongerQuest::Val1() const
{
	return db_.val1;
}

uint32 StrongerQuest::Val2() const
{
	return db_.val2;
}
StrongerGroup::StrongerGroup(const DB_StrongerGroup&db)
:db_(db)
{
	Utility::SplitStr2(db_.reward_list,reward_);
}

uint32 StrongerGroup::GroupId() const
{
	return db_.id;
}

uint32 StrongerGroup::OpenLevel() const
{
	return db_.open_level;
}

const IntPairVec& StrongerGroup::GetRewards() const
{
	return reward_;
}


StrongerTableMgr::StrongerTableMgr()
{
	FOREACH_DB_ITEM(stronger,DB_StrongerQuest)
	{
		StrongerQuest * stronger_quest = new StrongerQuest(*stronger);
		stronger_quest_map_[stronger_quest->QuestId()] = stronger_quest;
	}
	FOREACH_DB_ITEM(stronger_,DB_StrongerGroup)
	{
		StrongerGroup * stronger_group = new StrongerGroup(*stronger_);
		stronger_group_map_[stronger_group->GroupId()] = stronger_group;
	}
}


const IntPairVec* StrongerTableMgr::GetRewards(const uint32 group_id) const
{
	StrongerGroup * stronger_group = stronger_group_map_.pfind(group_id);
	if(stronger_group)
	{
		return &stronger_group->GetRewards();
	}
	return NULL;
}

const StrongerQuest* StrongerTableMgr::GetStrongerQuest(const uint32 type,const uint32 value1,const uint32 value2) const
{
	for(StrongerQuestMap::const_reverse_iterator iter = stronger_quest_map_.rbegin();iter != stronger_quest_map_.rend();++iter)
	{
		if(iter->second->QuestType() == type)
		{
			if(value1 >= iter->second->Val1() && value2 >= iter->second->Val2())
			{
				return iter->second;
			}
		}
	}
	return 0;
}

const StrongerQuestMap& StrongerTableMgr::GetStrongerQuestMap() const
{
	return stronger_quest_map_;
}

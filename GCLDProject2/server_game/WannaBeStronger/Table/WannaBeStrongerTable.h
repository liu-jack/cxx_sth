/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#ifndef INCLUDE_WANNA_BE_STRONGER__H
#define INCLUDE_WANNA_BE_STRONGER__H

#include "game_data/datastruct/struct_lottery_daily.h"
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
#include "SimpleSingleton.h"


class StrongerQuest
{
	const DB_StrongerQuest& db_;
public:
	StrongerQuest(const DB_StrongerQuest& db );
	uint32 QuestId() const;
	uint32 QuestGroup() const;
	uint32 QuestType()const;
	uint32 Val1() const;
	uint32 Val2() const;
};

class StrongerGroup
{
	const DB_StrongerGroup& db_;
	IntPairVec reward_;
public:
	StrongerGroup(const DB_StrongerGroup&db);
	uint32 GroupId() const;
	uint32 OpenLevel() const;
	const IntPairVec& GetRewards() const;
};


typedef PtrMap<uint32,StrongerQuest> StrongerQuestMap;
typedef PtrMap<uint32,StrongerGroup> StrongerGroupMap;
class StrongerTableMgr:public SimpleSingleton<StrongerTableMgr>
{
	StrongerQuestMap stronger_quest_map_;
	StrongerGroupMap stronger_group_map_;
public:
	StrongerTableMgr();
	const IntPairVec*GetRewards(const uint32 group_id) const;
	const StrongerQuest* GetStrongerQuest(const uint32 type,const uint32 value1,const uint32 value2) const;
	const StrongerQuestMap& GetStrongerQuestMap() const;
};


#define sStrongerTableMgr (StrongerTableMgr::Instance())


#endif //////INCLUDE_WANNA_BE_STRONGER__H